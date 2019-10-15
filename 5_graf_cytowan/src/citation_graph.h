#ifndef CITATION_GRAPH_H
#define CITATION_GRAPH_H

#include <map>
#include <vector>
#include <memory>
#include <set>

// Exceptions
class PublicationAlreadyCreated : public std::exception {
    const char *what() const noexcept override {
        return "PublicationAlreadyCreated";
    }
};

class PublicationNotFound : public std::exception {
    const char *what() const noexcept override {
        return "PublicationNotFound";
    }
};

class TriedToRemoveRoot : public std::exception {
    const char *what() const noexcept override {
        return "TriedToRemoveRoot";
    }
};

template<class Publication>
class CitationGraph {

private:
    // Nested class of Publications from CitationGraph
    class Node : std::enable_shared_from_this<Node> {

    private:

        Publication publication;

        // Sets of nodes representing citing and cited Publications
        std::set<std::weak_ptr<Node>, std::owner_less<std::weak_ptr<Node>>> parents;
        std::set<std::shared_ptr<Node>> children;

        // Pointer to map, where this node is stored
        std::map<typename Publication::id_type, std::weak_ptr<Node>> *publicationNodes;

        // Iterator to place in map, where this node is stored is stored
        typename std::map<typename Publication::id_type, std::weak_ptr<Node>>::iterator myMapIterator;

    public:

        // Node constructor
        explicit Node(typename Publication::id_type const &id,
                      std::shared_ptr<std::map<typename Publication::id_type, std::weak_ptr<Node>>> &map)
                : publication(Publication(id)),
                  publicationNodes(map.get()) {}

        // Node destructor - when Node is deleted, it erase itself from map
        ~Node() {

            std::weak_ptr<Node> toRemoveWeak = this->weak_from_this();
            publicationNodes->erase(myMapIterator);

            for (auto &child : this->children)
                child.get()->removeParent(toRemoveWeak);
        }

        // Getter for publication
        Publication &getPublication() {
            return this->publication;
        }

        // Setter for iterator to place in map
        void setMyMapIterator(
                typename std::map<typename Publication::id_type, std::weak_ptr<Node>>::iterator &myMapIterator) {
            this->myMapIterator = myMapIterator;
        }

        // Getter for parents
        const std::set<std::weak_ptr<Node>, std::owner_less<std::weak_ptr<Node>>> &getParents() const {
            return this->parents;
        }

        // Getter for children
        const std::set<std::shared_ptr<Node>> &getChildren() const {
            return this->children;
        }

        // Adder parent to set
        void addParent(std::weak_ptr<Node> &parent) {
            parents.insert(std::weak_ptr(parent));
        }

        // Adder child to set
        void addChild(std::shared_ptr<Node> &child) {
            children.insert(std::shared_ptr(child));
        }

        // Remover parent from set
        void removeParent(std::weak_ptr<Node> &parent) noexcept {
            parents.erase(parent);
        }

        // Remover child from set
        void removeChild(std::shared_ptr<Node> &child) noexcept {
            children.erase(child);
        }
    };

    // Map of Nodes, where id of Publication is a key
    std::shared_ptr<std::map<typename Publication::id_type, std::weak_ptr<Node>>> publicationNodes;

    // Shared pointer to the root
    std::shared_ptr<Node> root;

public:

    // CitationGraph constructor
    explicit CitationGraph(typename Publication::id_type const &stem_id) {

        publicationNodes = std::make_shared<std::map<typename Publication::id_type, std::weak_ptr<Node>>>();
        root = std::make_shared<Node>(stem_id, publicationNodes);
        auto pair = publicationNodes.get()->insert({stem_id, root});
        root.get()->setMyMapIterator(pair.first);
    }

    // CitationGraph move constructor
    CitationGraph(CitationGraph<Publication> &&other) noexcept {

        this->publicationNodes = other.publicationNodes;
        this->root = other.root;
        other.root = nullptr;
        other.publicationNodes = nullptr;
    }

    // Move assignment operator
    CitationGraph<Publication> &operator=(CitationGraph<Publication> &&other) noexcept {

        this->publicationNodes = other.publicationNodes;
        this->root = other.root;
        other.root = nullptr;
        other.publicationNodes = nullptr;

        return *this;
    }

    // Getter for root id
    typename Publication::id_type get_root_id() const noexcept(noexcept(std::declval<Publication>().get_id())) {
        return root.get()->getPublication().get_id();
    }

    // Getter for children of given Publication
    std::vector<typename Publication::id_type> get_children(typename Publication::id_type const &id) const {

        if (!exists(id))
            throw PublicationNotFound();

        std::vector<typename Publication::id_type> childrenIds;
        publicationNodes.get()->find(id);
        for (auto child : publicationNodes.get()->find(id)->second.lock().get()->getChildren())
            childrenIds.push_back(child.get()->getPublication().get_id());

        return childrenIds;
    }

    // Getter for parents of given Publication
    std::vector<typename Publication::id_type> get_parents(typename Publication::id_type const &id) const {

        if (!exists(id))
            throw PublicationNotFound();

        std::vector<typename Publication::id_type> parentIds;
        publicationNodes.get()->find(id);
        for (auto parent : publicationNodes->find(id)->second.lock().get()->getParents()) {
            if (!parent.expired())
                parentIds.push_back(parent.lock().get()->getPublication().get_id());
        }

        return parentIds;
    }

private:

    // Finds pointer to Publication of given id in map
    // If it is not there, returns pointer to end of map
    auto find(typename Publication::id_type const &id) const {

        auto nodeIterator = publicationNodes.get()->find(id);
        if ((nodeIterator != publicationNodes.get()->end()) && !(nodeIterator->second.expired())) {
            return nodeIterator;
        } else {
            return publicationNodes.get()->end();
        }
    }

public:

    // Returns if given Publication exist in map
    bool exists(typename Publication::id_type const &id) const {
        return !(find(id) == publicationNodes.get()->end());
    }

    // Returns reference to Publication of given id
    Publication &operator[](typename Publication::id_type const &id) const {

        auto publication = find(id);

        if (publication == publicationNodes.get()->end())
            throw PublicationNotFound();

        std::shared_ptr<Node> nodePtr = publication->second.lock();
        return nodePtr.get()->getPublication();
    };

    // Creates new Node in map with Publication of given id, with given parents
    void create(typename Publication::id_type const &id, std::vector<typename Publication::id_type> const &parent_ids) {

        if (exists(id))
            throw PublicationAlreadyCreated();

        for (auto parentId : parent_ids) {
            if (!exists(parentId))
                throw PublicationNotFound();
        }

        std::shared_ptr<Node> newNode = std::make_shared<Node>(id, publicationNodes);

        for (auto &parentId : parent_ids)
            newNode.get()->addParent(publicationNodes.get()->find(parentId)->second);

        std::set<std::weak_ptr<Node>, std::owner_less<std::weak_ptr<Node>>> alreadyAddedTo;
        try {

            for (auto &parentId : parent_ids) {

                std::weak_ptr<Node> parent(publicationNodes.get()->find(parentId)->second);
                alreadyAddedTo.insert(parent);
                parent.lock().get()->addChild(newNode);
            }

            auto pair = publicationNodes.get()->insert({id, std::weak_ptr(newNode)});
            newNode.get()->setMyMapIterator(pair.first);

        } catch (std::exception &e) {

            for (auto &parent : alreadyAddedTo)
                parent.lock().get()->removeChild(newNode);
            throw e;
        }
    }

    // Creates new Node in map with Publication of given id, with given parent
    void create(typename Publication::id_type const &id, typename Publication::id_type const &parent_id) {

        std::vector<typename Publication::id_type> singleParentId;
        singleParentId.push_back(parent_id);
        create(id, singleParentId);
    }

    // Adds new link between two given Publications from map
    void add_citation(typename Publication::id_type const &child_id, typename Publication::id_type const &parent_id) {

        if (!exists(child_id) || !exists(parent_id))
            throw PublicationNotFound();

        std::shared_ptr<Node> child((*publicationNodes.get())[child_id]);
        std::weak_ptr<Node> parent((*publicationNodes.get())[parent_id]);

        child.get()->addParent(parent);

        try {
            parent.lock()->addChild(child);
        } catch (std::exception &e) {
            child.get()->removeParent(parent);
            throw e;
        }
    }

    // Removes Publication of given id from map
    void remove(typename Publication::id_type const &id) {

        if (!exists(id))
            throw PublicationNotFound();

        if (get_root_id() == id)
            throw TriedToRemoveRoot();

        std::shared_ptr<Node> toRemove((*publicationNodes.get())[id]);

        for (auto &parent : toRemove.get()->getParents())
            parent.lock().get()->removeChild(toRemove);
    }

};

#endif //CITATION_GRAPH_H
