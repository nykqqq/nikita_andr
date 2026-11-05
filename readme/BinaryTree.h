#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QQueue>
#include <QVector>
#include <QDebug>

// Узел дерева
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    BinaryTree();
    ~BinaryTree();

    // Загрузка из файла (послойный обход)
    bool loadFromFile(const QString& filename);

    // Префиксный обход (УЛП) - Возвращает строку
    QString getPreorderTraversal();

    // Подсчет высоты
    int getHeight();

    // Для отрисовки: получение корня
    Node* getRoot() { return root; }

    // Очистка
    void clear();

private:
    Node* root;

    // Вспомогательные рекурсивные функции
    void deleteTree(Node* node);
    void preorder(Node* node, QString& result);
    int height(Node* node);
};

#endif // BINARYTREE_H
