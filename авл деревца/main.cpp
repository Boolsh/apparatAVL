#include <iostream>

using TInfo = int;

struct AVLNode
{
    TInfo info;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(TInfo info) : info(info), left(nullptr), right(nullptr), height(1) {}
};

struct AVLTree
{
    AVLNode* root = nullptr;

    int height(AVLNode* node)
    {
        return (node == nullptr) ? 0 : node->height;
    }

    int getBalance(AVLNode* node)
    {
        return (node == nullptr) ? 0 : height(node->left) - height(node->right);
    }

    /*поворот вправо вокруг узла x при вставке, когда левое поддерево узла x слишком высокое по сравнению с правым.
   после поворота x становится правым дочерним узлом нового корня y, а y становится новым корнем поддерева. */
    AVLNode* rightRotate(AVLNode* x)
    {
        AVLNode* result = x;
        if (x != nullptr)
        {
            AVLNode* y = x->left;//новый корень
            AVLNode* T = y->right;//tmp
            y->right = x;//поворот
            x->left = T;//поворот
            //обновляем значения
            x->height = std::max(height(x->left), height(x->right)) + 1;
            y->height = std::max(height(y->left), height(y->right)) + 1;
            result = y;
        }
        return result;
    }

    /*поворот влево вокруг узла x при вставке, когда правое поддерево узла x слишком высокое по сравнению с левым.
    после поворота x становится левым дочерним узлом нового корня y, а y становится новым корнем поддерева. */
    AVLNode* leftRotate(AVLNode* x)
    {
        AVLNode* result = x;
        if (x)
        {
            AVLNode* y = x->right; //новый корень поддерева
            AVLNode* T = y->left;  //временное поддерево
            //сам поворот
            y->left = x;
            x->right = T;
            //обновление высот узлов
            x->height = std::max(height(x->left), height(x->right)) + 1;
            y->height = std::max(height(y->left), height(y->right)) + 1;
            result = y;  // Новый корень
        }
        return result;
    }

    //большой правый поворот делаем когда левое поддерево узла имеет правое поддерево, 
    // которое слишком высокое.
    AVLNode* bigRightRotate(AVLNode* node)
    {
        node->left = leftRotate(node->left);//левый поворот на левом поддереве
        return rightRotate(node); //правый поворот на текущем узле
    }

    // Большой правый поворот делаем когда когда правое поддерево узла имеет левое поддерево, 
    // которое слишком высокое.
    AVLNode* bigLeftRotate(AVLNode* node)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }



    AVLNode* insert(AVLNode* node, TInfo info)
    {
        if (!node)
            node = new AVLNode(info);
        else
        {
            if (info < node->info)
                node->left = insert(node->left, info);
            else if (info > node->info)
                node->right = insert(node->right, info);

            node->height = 1 + std::max(height(node->left), height(node->right));
            int balance = getBalance(node);
            if (balance > 1 && info < node->left->info)
                node = rightRotate(node);
            else if (balance < -1 && info > node->right->info)
                node = leftRotate(node);
            else if (balance > 1 && info > node->left->info)
                node = bigRightRotate(node);
            else if (balance < -1 && info < node->right->info)
                node = bigLeftRotate(node);
        }
        return node;
    }

    AVLNode* deleteNode(AVLNode* node, TInfo info)
    {
        AVLNode* result = node; // Переменная для возврата

        //Обычное удаление в бинарном дереве поиска
        if (node != nullptr)
        {
            if (info < node->info)
            {
                node->left = deleteNode(node->left, info);
            }
            else if (info> node->info)
            {
                node->right = deleteNode(node->right, info);
            }
            else
            {
                // Узел найден

                // Узел с одним или без потомков
                if (node->left == nullptr || node->right == nullptr)
                {
                    AVLNode* temp = (node->left != nullptr) ? node->left : node->right;

                    // Узел без потомков
                    if (temp == nullptr)
                    {
                        temp = node;
                        result = nullptr;
                    }
                    else // Узел с одним потомком
                    {
                        *node = *temp; // Копируем данные потомка
                        result = node;
                    }
                    delete temp;
                }
                else
                {
                    // Узел с двумя потомками: находим наименьший узел в правом поддереве
                    AVLNode* temp = minValueNode(node->right);

                    // Копируем значение наименьшего узла в текущий узел
                    node->info = temp->info;

                    // Удаляем наименьший узел
                    node->right = deleteNode(node->right, temp->info);
                    result = node;
                }
            }

            // Обновляем высоту узла
            if (result != nullptr)
            {
                result->height = 1 + std::max(height(result->left), height(result->right));

                int balance = getBalance(result);

                // LL случай
                if (balance > 1 && getBalance(result->left) >= 0)
                    result = rightRotate(result);

                // LR случай
                else if (balance > 1 && getBalance(result->left) < 0)
                {
                    //result->left = leftRotate(result->left);
                    //result = rightRotate(result);

                    result = bigRightRotate(result);
                }

                // RR случай
                else if (balance < -1 && getBalance(result->right) <= 0)
                    result = leftRotate(result);

                // RL случай
                else if (balance < -1 && getBalance(result->right) > 0)
                {

                    //result->right = rightRotate(result->right);
                    //result = leftRotate(result);

                    result = bigLeftRotate(result);
                }
            }
        }

        return result;
    }

    AVLNode* minValueNode(AVLNode* node)
    {
        AVLNode* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    void inorder(AVLNode* node)
    {
        if (node != nullptr)
        {
            inorder(node->left);
            std::cout << node->info << " ";
            inorder(node->right);
        }
    }

    void printTree(AVLNode* node, int space = 0, int indent = 7)
    {
        if (node != nullptr)
        {
            space += indent;
            printTree(node->right, space);
            std::cout << '\n';
            for (int i = indent; i < space; i++)
                std::cout << " ";
            std::cout << node->info << "(" << node->height << ")" << '\n';
            printTree(node->left, space);
        }
    }

    AVLNode* get_root() { return root; }
};

void main()
{
    setlocale(LC_ALL, "Russian");
    AVLTree tree;
    AVLNode* root = tree.get_root();

    //// Вставка элементов в AVL-дерево
    //root = tree.insert(root, 10);
    //root = tree.insert(root, 8);
    //root = tree.insert(root, 6);
    //root = tree.insert(root, 9);
    //root = tree.insert(root, 2);
    //root = tree.insert(root, 7);
    //root = tree.insert(root, 12);
    //root = tree.insert(root, 11);

    //std::cout << "Дерево до удаления узлов:\n";
    //tree.printTree(root);  // Вывод дерева до удаления

    //std::cout << "\n===============================================\n";

    //// Удаление элементов
    //root = tree.deleteNode(root, 2);  // Удаление узла с одним потомком или без потомков
    //std::cout << "Дерево после удаления 20:\n";
    //tree.printTree(root);  // Вывод дерева после удаления узла 20

    //std::cout << "\n===============================================\n";

    //root = tree.deleteNode(root, 7);  // Удаление узла с двумя потомками
    //std::cout << "Дерево после удаления 60:\n";
    //tree.printTree(root);  // Вывод дерева после удаления узла 60

    //std::cout << "\n===============================================\n";


   for (int i = 0; i < 16; ++i )
       root = tree.insert(root, i);

   tree.printTree(root);
   std::cout << "\n===============================================\n";

   root = tree.deleteNode(root, 13);

   tree.printTree(root);
   std::cout << "\n===============================================\n";

}