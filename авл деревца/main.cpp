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

    /*������� ������ ������ ���� x ��� �������, ����� ����� ��������� ���� x ������� ������� �� ��������� � ������.
   ����� �������� x ���������� ������ �������� ����� ������ ����� y, � y ���������� ����� ������ ���������. */
    AVLNode* rightRotate(AVLNode* x)
    {
        AVLNode* result = x;
        if (x != nullptr)
        {
            AVLNode* y = x->left;//����� ������
            AVLNode* T = y->right;//tmp
            y->right = x;//�������
            x->left = T;//�������
            //��������� ��������
            x->height = std::max(height(x->left), height(x->right)) + 1;
            y->height = std::max(height(y->left), height(y->right)) + 1;
            result = y;
        }
        return result;
    }

    /*������� ����� ������ ���� x ��� �������, ����� ������ ��������� ���� x ������� ������� �� ��������� � �����.
    ����� �������� x ���������� ����� �������� ����� ������ ����� y, � y ���������� ����� ������ ���������. */
    AVLNode* leftRotate(AVLNode* x)
    {
        AVLNode* result = x;
        if (x)
        {
            AVLNode* y = x->right; //����� ������ ���������
            AVLNode* T = y->left;  //��������� ���������
            //��� �������
            y->left = x;
            x->right = T;
            //���������� ����� �����
            x->height = std::max(height(x->left), height(x->right)) + 1;
            y->height = std::max(height(y->left), height(y->right)) + 1;
            result = y;  // ����� ������
        }
        return result;
    }

    //������� ������ ������� ������ ����� ����� ��������� ���� ����� ������ ���������, 
    // ������� ������� �������.
    AVLNode* bigRightRotate(AVLNode* node)
    {
        node->left = leftRotate(node->left);//����� ������� �� ����� ���������
        return rightRotate(node); //������ ������� �� ������� ����
    }

    // ������� ������ ������� ������ ����� ����� ������ ��������� ���� ����� ����� ���������, 
    // ������� ������� �������.
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
        AVLNode* result = node; // ���������� ��� ��������

        //������� �������� � �������� ������ ������
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
                // ���� ������

                // ���� � ����� ��� ��� ��������
                if (node->left == nullptr || node->right == nullptr)
                {
                    AVLNode* temp = (node->left != nullptr) ? node->left : node->right;

                    // ���� ��� ��������
                    if (temp == nullptr)
                    {
                        temp = node;
                        result = nullptr;
                    }
                    else // ���� � ����� ��������
                    {
                        *node = *temp; // �������� ������ �������
                        result = node;
                    }
                    delete temp;
                }
                else
                {
                    // ���� � ����� ���������: ������� ���������� ���� � ������ ���������
                    AVLNode* temp = minValueNode(node->right);

                    // �������� �������� ����������� ���� � ������� ����
                    node->info = temp->info;

                    // ������� ���������� ����
                    node->right = deleteNode(node->right, temp->info);
                    result = node;
                }
            }

            // ��������� ������ ����
            if (result != nullptr)
            {
                result->height = 1 + std::max(height(result->left), height(result->right));

                int balance = getBalance(result);

                // LL ������
                if (balance > 1 && getBalance(result->left) >= 0)
                    result = rightRotate(result);

                // LR ������
                else if (balance > 1 && getBalance(result->left) < 0)
                {
                    //result->left = leftRotate(result->left);
                    //result = rightRotate(result);

                    result = bigRightRotate(result);
                }

                // RR ������
                else if (balance < -1 && getBalance(result->right) <= 0)
                    result = leftRotate(result);

                // RL ������
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

    //// ������� ��������� � AVL-������
    //root = tree.insert(root, 10);
    //root = tree.insert(root, 8);
    //root = tree.insert(root, 6);
    //root = tree.insert(root, 9);
    //root = tree.insert(root, 2);
    //root = tree.insert(root, 7);
    //root = tree.insert(root, 12);
    //root = tree.insert(root, 11);

    //std::cout << "������ �� �������� �����:\n";
    //tree.printTree(root);  // ����� ������ �� ��������

    //std::cout << "\n===============================================\n";

    //// �������� ���������
    //root = tree.deleteNode(root, 2);  // �������� ���� � ����� �������� ��� ��� ��������
    //std::cout << "������ ����� �������� 20:\n";
    //tree.printTree(root);  // ����� ������ ����� �������� ���� 20

    //std::cout << "\n===============================================\n";

    //root = tree.deleteNode(root, 7);  // �������� ���� � ����� ���������
    //std::cout << "������ ����� �������� 60:\n";
    //tree.printTree(root);  // ����� ������ ����� �������� ���� 60

    //std::cout << "\n===============================================\n";


   for (int i = 0; i < 16; ++i )
       root = tree.insert(root, i);

   tree.printTree(root);
   std::cout << "\n===============================================\n";

   root = tree.deleteNode(root, 13);

   tree.printTree(root);
   std::cout << "\n===============================================\n";

}