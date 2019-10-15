#ifndef TST_H
#define TST_H

#include <memory>

namespace Detail {

    template<typename Iter, typename Acc, typename Functor>
    Acc fold(Iter first, Iter last, Acc acc, Functor functor) {
        return (first == last ? acc : fold(first + 1, last, functor(acc, *(first)), functor));
    }
}

template<typename C = char>
class TST {

private:

    const std::shared_ptr<TST<C>> leftNode;
    const std::shared_ptr<TST<C>> centerNode;
    const std::shared_ptr<TST<C>> rightNode;
    const bool isEndOfWord = false;
    const C data = '\0';

    auto &emptyNode() const {
        static auto dummy = std::make_shared<TST<C>>();
        return dummy;
    }

    TST(std::shared_ptr<TST<C>> left, std::shared_ptr<TST<C>> center, std::shared_ptr<TST<C>> right, bool isEndOfWord, C data) :
        leftNode(std::move(left)),
        centerNode(std::move(center)),
        rightNode(std::move(right)),
        isEndOfWord(isEndOfWord),
        data(data) {}

    /*
    * Calculates the length of the longest prefix of str and words in tree.
    */
    size_t prefixHelper(const C* str) const {
        return (str[0] == '\0' || empty() ? 0 :
               (data == str[0] ? (1 + centerNode->prefixHelper(str + 1)) :
               (str[0] < data ? leftNode->prefixHelper(str) : rightNode->prefixHelper(str))));
    }

public:

    TST() = default;

    explicit TST(const std::basic_string<C>& str) : TST(str.c_str()) {}

    explicit TST(const C* str) :
        leftNode(emptyNode()),
        centerNode(str[0] == '\0' || str[1] == '\0' ? emptyNode() : std::make_shared<TST<C>>(str + 1)),
        rightNode(emptyNode()),
        isEndOfWord(str[0] == '\0' ? false : str[1] == '\0'),
        data(str[0]) {}

    /*
     * Adds word to tree.
     */
    TST operator+(const std::basic_string<C>& str) const {
        return this->operator+(str.c_str());
    }

    TST operator+(const C* str) const {
        return empty() ? TST(str) : (str[0] == '\0' ? *this :
                                    (str[0] == data ? TST(leftNode, std::make_shared<TST<C>>(center() + (str + 1)), rightNode, str[1] == '\0', data) :
                                    (str[0] < data ? TST(std::make_shared<TST<C>>(left() + str), centerNode, rightNode, isEndOfWord, data) :
                                    TST(leftNode, centerNode, std::make_shared<TST<C>>(right() + str), isEndOfWord, data))));
    }

    /*
     * Returns the character stored in the node.
     */
    C value() const {
        return empty() ? throw std::logic_error("value_error") : data;
    }

    /*
     * Checks if a node represents the end of a word.
     */
    bool word() const {
        return empty() ? throw std::logic_error("word_error") : isEndOfWord;
    }

    /*
     * Returns the left child of a node.
     */
    TST left() const {
        return empty() ? throw std::logic_error("left_error") : *(leftNode);
    }

    /*
     * Returns the center child of the node.
     */
    TST center() const {
        return empty() ? throw std::logic_error("center_error") : *(centerNode);
    }

    /*
     * Returns the right child of the node.
     */
    TST right() const {
        return  empty() ? throw std::logic_error("right_error") : *(rightNode);
    }

    /*
     * Checks if a node is empty.
     */
    bool empty() const {
        return data == '\0';
    }

    bool exist(const C* str) const {
        return (str[0] == '\0' || empty()) ? false :
               ((str + 1)[0] == '\0' && str[0] == data ? isEndOfWord :
                (str[0] == data ? centerNode->exist(str + 1) :
                 (str[0] < data ? leftNode->exist(str) : rightNode->exist(str))));
    }

    /*
     * Checks if word str is in the tree.
     */
    bool exist(const std::basic_string<C>& str) const {
        return exist(str.c_str());
    }

    /*
     * Returns the longest prefix of str and words in tree.
     */
    std::basic_string<C> prefix(const std::basic_string<C>& str) const {
        return str.substr(0, prefixHelper(str.c_str()));
    }

    template<typename Acc, typename Functor>
    Acc fold(Acc acc, Functor functor) const {
        return empty() ? acc : functor(left().fold(center().fold(right().fold(acc, functor), functor), functor), value());
    }

    /*
     * Returns number of nodes in tree.
     */
    size_t size() const {
        return this->fold(size_t(), [](size_t acc, C){
            return 1 + acc;
        });
    }

};

#endif //TST_H
