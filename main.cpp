#include <iostream>
#include <initializer_list>

template <class T> class Set
{
private:
    struct node
    {
        node* left;
        node* right;
        node* par;
        node* mx;
        T key;
        int height;
        int sz;

        node(T val)
        {
            key = val;
            height = 1;
            sz = 1;
            left = nullptr;
            right = nullptr;
            par = nullptr;
            mx = nullptr;
        }
    };

    node* root = nullptr;

    int get_height(node* root)
    {
        if (root == nullptr)
            return 0;
        return root->height;
    }

    int get_size(node* root) const
    {
        if (root == nullptr)
            return 0;
        return root->sz;
    }

    void update(node* root)
    {
        if (root == nullptr)
            return;
        if (get_height(root->left) > get_height(root->right))
            root->height = get_height(root->left);
        else
            root->height = get_height(root->right);
        if ((root->right == nullptr) || (root->right->mx == nullptr))
            root->mx = root->right;
        else
            root->mx = root->right->mx;
        root->height++;
        root->sz = 1 + get_size(root->left) + get_size(root->right);
    }

    node* rotate_left(node* root)
    {
        node* new_root = root->right;
        new_root->par = root->par;
        root->right = new_root->left;
        new_root->left = root;
        root->par = new_root;
        if (root->right != nullptr)
            root->right->par = root;
        update(root);
        update(new_root);
        return new_root;
    }

    node* rotate_right(node* root)
    {
        node* new_root = root->left;
        new_root->par = root->par;
        root->left = new_root->right;
        new_root->right = root;
        root->par = new_root;
        if (root->left != nullptr)
            root->left->par = root;
        update(root);
        update(new_root);
        return new_root;
    }

    int delta(node* root)
    {
        return get_height(root->left) - get_height(root->right);
    }

    void upd_right(node* root)
    {
        if (root->right != nullptr)
        {
            root->right->par = root;
            if (root->right->mx == nullptr)
            {
                root->mx = root->right;
            }
            else
            {
                root->mx = root->right->mx;
            }
        }
        else
        {
            root->mx = nullptr;
        }
    }

    void upd_left(node* root)
    {
        if (root->left != nullptr)
            root->left->par = root;
    }

    node* balance(node* root)
    {
        update(root);
        if (delta(root) == -2)
        {
            if (delta(root->right) > 0)
            {
                root->right = rotate_right(root->right);
                upd_right(root);
            }
            return rotate_left(root);
        }
        else if (delta(root) == 2)
        {
            if (delta(root->left) < 0)
            {
                root->left = rotate_left(root->left);
                upd_left(root);
            }
            return rotate_right(root);
        }
        else
        {
            return root;
        }
    }

    node* insert(node* root, T val)
    {
        if (root == nullptr)
        {
            return (new node(val));
        }
        if (val < root->key)
        {
            root->left = insert(root->left, val);
            upd_left(root);
        }
        else if (root->key < val)
        {
            root->right = insert(root->right, val);
            upd_right(root);
        }
        return balance(root);
    }

    node* down_min(node* root)
    {
        if (root->left != nullptr)
        {
            return down_min(root->left);
        }
        else
        {
            return root;
        }
    }

    node* erase_min(node* root)
    {
        if (root->left == nullptr)
        {
            return root->right;
        }
        root->left = erase_min(root->left);
        upd_left(root);
        return balance(root);
    }

    node* erase(node* root, T val)
    {
        if (root == nullptr)
            return nullptr;
        if (val < root->key)
        {
            root->left = erase(root->left, val);
            upd_left(root);
        }
        else if (root->key < val)
        {
            root->right = erase(root->right, val);
            upd_right(root);
        }
        else
        {
            node* new_root;
            node* ltree = root->left;
            node* rtree = root->right;
            node* root_par = root->par;
            delete root;
            if (rtree == nullptr)
            {
                return ltree;
            }
            else
            {
                new_root = down_min(rtree);
                new_root->right = erase_min(rtree);
                new_root->left = ltree;
                new_root->par = root_par;
                upd_right(new_root);
                upd_left(new_root);
                return balance(new_root);
            }
        }
        return balance(root);
    }

    void print(node* root)
    {
        if (root == nullptr)
            return;
        print(root->left);
        std::cout << root->key << " ";
        print(root->right);
    }

    node* copy(node* other_set, node* root)
    {
        root = new node(other_set->key);
        root->height = other_set->height;
        root->sz = other_set->sz;
        root->par = other_set->par;
        root->mx = other_set->mx;
        if (other_set->left != nullptr)
        {
            root->left = copy(other_set->left, root->left);
            upd_left(root);
        }
        if (other_set->right != nullptr)
        {
            root->right = copy(other_set->right, root->right);
            if (root->right != nullptr)
                root->right->par = root;
        }
        return root;
    }

    void destroy(node* root)
    {
        if (root == nullptr)
            return;
        if (root->left != nullptr)
            destroy(root->left);
        if (root->right != nullptr)
            destroy(root->right);
        delete root;
        return;
    }

    node* find(node* root, T val) const
    {
        if (root == nullptr)
            return nullptr;
        if (val < root->key)
        {
            return find(root->left, val);
        }
        else if (root->key < val)
        {
            return find(root->right, val);
        }
        else
        {
            return root;
        }
    }

    node* lower_bound(node* root, T val) const
    {
        if (root == nullptr)
            return nullptr;
        if (root->key < val)
        {
            return lower_bound(root->right, val);
        }
        else
        {
            auto res = lower_bound(root->left, val);
            if (res == nullptr)
                return root;
            else
                return res;
        }
    }

    int best(node* root)
    {
        if (root->right == nullptr)
            return root->key;
        return best(root->right);
    }

public:

    class iterator
    {
    public:
        iterator()
        {
            ptr = nullptr;
            past = nullptr;
        }

        ~iterator()
        {

        }

        iterator(node* root, bool flag = false)
        {
            if (flag == false)
            {
                ptr = root;
                past = nullptr;
            }
            else
            {
                ptr = nullptr;
                past = root;
            }
        }

        iterator(const node* root, bool flag = false)
        {
            if (flag == false)
            {
                ptr = root;
                past = nullptr;
            }
            else
            {
                ptr = nullptr;
                past = root;
            }
        }

        iterator(const iterator& it)
        {
            ptr = it.ptr;
            past = it.past;
        }

        iterator& operator=(const iterator& it)
        {
            ptr = it.ptr;
            past = it.past;
            return *this;
        }

        bool operator==(const iterator& it) const
        {
            return ptr == it.ptr;
        }

        bool operator!=(const iterator& it) const
        {
            return ptr != it.ptr;
        }

        iterator& operator++()
        {
            if (ptr == nullptr)
            {
                return *this;
            }
            if (ptr->right != nullptr)
            {
                past = ptr;
                ptr = ptr->right;
                while (ptr->left != nullptr)
                {
                    past = ptr;
                    ptr = ptr->left;
                }
            }
            else
            {
                auto prev = ptr;
                ptr = ptr->par;
                while ((ptr != nullptr) && (ptr->right == prev))
                {
                    prev = ptr;
                    ptr = ptr->par;
                }
                if (ptr != nullptr)
                {
                    past = prev;
                }
                else
                {
                    past = prev->mx;
                }
            }
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        iterator& operator--()
        {
            if (ptr == nullptr)
            {
                ptr = past;
                past = nullptr;
                return *this;
            }
            if (ptr->left != nullptr)
            {
                past = ptr;
                ptr = ptr->left;
                while (ptr->right != nullptr)
                {
                    past = ptr;
                    ptr = ptr->right;
                }
            }
            else
            {
                auto prev = ptr;
                ptr = ptr->par;
                while ((ptr != nullptr) && (ptr->left == prev))
                {
                    prev = ptr;
                    ptr = ptr->par;
                }
                past = prev;
            }
            return *this;
        }

        iterator operator--(int)
        {
            iterator tmp(*this);
            --(*this);
            return tmp;
        }

        T operator*() const
        {
            return ptr->key;
        }

        T* operator->() const
        {
            return &(ptr->key);
        }

    private:
        node *ptr = nullptr;
        node *past = nullptr;
    };

    Set()
    {
        root = nullptr;
    }

    template<typename iterator> Set(iterator begin, iterator end)
    {
        root = nullptr;
        for (auto iter = begin; iter != end; iter++)
        {
            root = insert(root, *iter);
        }
    }

    Set(std::initializer_list<T> list)
    {
        root = nullptr;
        for (auto value : list)
        {
            root = insert(root, value);
        }
    }

    Set(const Set<T> &other_set)
    {
        if (root != nullptr)
            destroy(root);
        for (const T & value : other_set)
            root = insert(root, value);
    }

    bool operator!=(const Set<T> &other_set) const
    {
        return (root == other_set.root);
    }

    Set& operator= (const Set<T> &other_set)
    {
        if (this == &other_set)
        {
            return *this;
        }
        if (root != nullptr)
            destroy(root);
        root = nullptr;
        for (const T & value : other_set)
            root = insert(root, value);
        return *this;
    }

    ~Set()
    {
        destroy(root);
    }

    void insert(T val)
    {
        root = insert(root, val);
    }

    void erase(T val)
    {
        root = erase(root, val);
    }

    int size() const
    {
        return get_size(root);
    }

    bool empty() const
    {
        if (root == nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    iterator begin() const
    {
        node* it = root;
        if (it == nullptr)
            return iterator(it);
        while (it->left != nullptr)
            it = it->left;
        return iterator(it);
    }

    iterator end() const
    {
        node* it = root;
        if (it == nullptr)
            return iterator(it, true);
        while (it->right != nullptr)
            it = it->right;
        return iterator(it, true);
    }

    iterator find(T val) const
    {
        node* res = find(root, val);
        if (res == nullptr)
        {
            return end();
        }
        else
        {
            return iterator(res);
        }
    }

    iterator lower_bound(T val) const
    {
        node* res = lower_bound(root, val);
        if (res == nullptr)
        {
            return end();
        }
        else
        {
            return iterator(res);
        }
    }

    void print()
    {
        print(root);
    }

    int best()
    {
        return best(root);
    }
};
