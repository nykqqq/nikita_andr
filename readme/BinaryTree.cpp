#include "BinaryTree.h"

BinaryTree::BinaryTree() : root(nullptr) {}
BinaryTree::~BinaryTree() { clear(); }

void BinaryTree::clear() {
    deleteTree(root);
    root = nullptr;
}

void BinaryTree::deleteTree(Node* node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

bool BinaryTree::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    clear();
    QTextStream in(&file);

    QQueue<Node*> queue;
    bool isRoot = true;

    // Для обработки прочерков (чтобы знать, левый занят или правый)
    // Мы будем хранить состояние для каждого родителя в очереди
    QQueue<QPair<bool, bool>> childrenStates; // (hasLeft, hasRight)

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList tokens = line.split(" ", Qt::SkipEmptyParts);

        for (const QString& token : tokens) {
            if (isRoot) {
                if (token == "-") continue; // корня не может не быть
                bool ok;
                int val = token.toInt(&ok);
                if (!ok) continue;

                root = new Node(val);
                isRoot = false;
                queue.enqueue(root);
                childrenStates.enqueue({false, false}); // у корня пока нет детей
            } else {
                // Берем первого родителя из очереди
                Node* parent = queue.head();
                bool hasLeft = childrenStates.head().first;
                bool hasRight = childrenStates.head().second;

                if (token == "-") {
                    // Если прочерк
                    if (!hasLeft) {
                        // Это место левого ребенка. Он "отсутствует"
                        childrenStates.head().first = true; // левый занят
                    } else {
                        // Это место правого. Родитель заполнен, убираем из очереди
                        queue.dequeue();
                        childrenStates.dequeue();
                    }
                } else {
                    // Это число, создаем узел
                    bool ok;
                    int val = token.toInt(&ok);
                    if (!ok) continue;

                    Node* newNode = new Node(val);

                    if (!hasLeft) {
                        parent->left = newNode;
                        childrenStates.head().first = true; // левый занят
                    } else {
                        parent->right = newNode;
                        queue.dequeue(); // родитель полностью заполнен
                        childrenStates.dequeue();
                    }

                    // Новый узел встает в очередь
                    queue.enqueue(newNode);
                    childrenStates.enqueue({false, false});
                }
            }
        }
    }
    file.close();
    return true;
}

void BinaryTree::preorder(Node* node, QString& result) {
    if (!node) return;
    result += QString::number(node->data) + " ";
    preorder(node->left, result);
    preorder(node->right, result);
}

QString BinaryTree::getPreorderTraversal() {
    QString result;
    preorder(root, result);
    return result.trimmed();
}

int BinaryTree::height(Node* node) {
    if (!node) return 0;
    int leftH = height(node->left);
    int rightH = height(node->right);
    return (leftH > rightH ? leftH : rightH) + 1;
}

int BinaryTree::getHeight() { return height(root); }
