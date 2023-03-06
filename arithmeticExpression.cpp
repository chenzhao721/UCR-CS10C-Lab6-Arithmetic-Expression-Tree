#include <stack>
#include <sstream>
#include <iostream>
#include <fstream>
#include "arithmeticExpression.h"

int inorderIndex=1;

arithmeticExpression::arithmeticExpression(const string &expression)
{
    for(unsigned i=0;i<expression.size();i++)
    {
        infixExpression.push_back(expression.at(i));
    }

}

int arithmeticExpression::priority(char op){
    int priority = 0;
    if(op == '('){
        priority =  3;
    }
    else if(op == '*' || op == '/'){
        priority = 2;
    }
    else if(op == '+' || op == '-'){
        priority = 1;
    }
    return priority;
}

string arithmeticExpression::infix_to_postfix(){
    stack<char> s;
    ostringstream oss;
    char c;
    for(unsigned i = 0; i< infixExpression.size();++i){
        c = infixExpression.at(i);
        if(c == ' '){
            continue;
        }
        if(c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'){ //c is an operator
            if( c == '('){
                s.push(c);
            }
            else if(c == ')'){
                while(s.top() != '('){
                    oss << s.top();
                    s.pop();
                }
                s.pop();
            }
            else{
                while(!s.empty() && priority(c) <= priority(s.top())){
                    if(s.top() == '('){
                        break;
                    }
                    oss << s.top();
                    s.pop();
                }
                s.push(c);
            }
        }
        else{ //c is an operand
            oss << c;
        }
    }
    while(!s.empty()){
        oss << s.top();
        s.pop();
    }
    return oss.str();
}

void arithmeticExpression::visualizeTree(const string &outputFilename){
    ofstream outFS(outputFilename.c_str());
    if(!outFS.is_open()){
        cout<<"Error opening "<< outputFilename<<endl;
        return;
    }
    outFS<<"digraph G {"<<endl;
    visualizeTree(outFS,root);
    outFS<<"}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0,outputFilename.size()-4)+".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}


void arithmeticExpression::visualizeTree(ofstream &outFS, TreeNode *root)
{

    printNode(outFS, root);
    printEdge(outFS, root);

    return;

}

void arithmeticExpression::printNode(ofstream &outFS, TreeNode *root)
{
    if(!root) return;
    printNode(outFS, root->left);
    outFS<<root->key<<" [color = red, peripheries=2, style = filled, label=\"key="<<root->data<<
    ", in="<<inorderIndex<<"\"];"<<endl;
    inorderIndex++;
    printNode(outFS, root->right);

}


void arithmeticExpression::printEdge(ofstream &outFS, TreeNode *root)
{
    if(!root) return;
    
    if(root->left) outFS<<root->key<<" -> "<<root->left->key<<";"<<endl;
    if(root->right) outFS<<root->key<<" -> "<<root->right->key<<";"<<endl;

    printEdge(outFS, root->left);
    printEdge(outFS, root->right);

    return;

}



void arithmeticExpression::buildTree()
{
    stack<TreeNode*> tree;
    string postfixExpression = infix_to_postfix();
    char nodekey = 'a';
    for(unsigned i=0;i<postfixExpression.size();i++)
    {
        char c = postfixExpression.at(i);
        if(isOperand(c))
        {
            TreeNode *leafnode = new TreeNode(c, nodekey);
            tree.push(leafnode);
        }

        else if(isOperator(c))
        {
            TreeNode *node = new TreeNode(c, nodekey);
            node->right = tree.top();
            tree.pop();
            node->left = tree.top();
            tree.pop();
            tree.push(node);
        }

        else
        {
            cout<<"error input"<<endl;
        }

        nodekey++;

    }

    root = tree.top();
    tree.pop();
}

bool arithmeticExpression::isOperand(char c)
{
    if(c<='z' && c>='a') return true;
    else return false;
}

bool arithmeticExpression::isOperator(char c)
{
    if(c=='+' || c=='-' || c=='*' || c=='/' || c=='=') return true;
    else return false;
}


void arithmeticExpression::infix()
{
    infix(root);

}

void arithmeticExpression::infix(TreeNode *root)
{
    if(!root) return;
    if(root->left && root->right) cout<<"(";
    infix(root->left);
    cout<<root->data;
    infix(root->right);
    if(root->left && root->right) cout<<")";
    return;
}
void arithmeticExpression::prefix()
{
    prefix(root);
}

void arithmeticExpression::prefix(TreeNode *root)
{
    if(!root) return;
    cout<<root->data;
    prefix(root->left);
    prefix(root->right);
    return;

}
void arithmeticExpression::postfix()
{
    postfix(root);

}

void arithmeticExpression::postfix(TreeNode *root)
{
    if(!root) return;
    postfix(root->left);
    postfix(root->right);
    cout<<root->data;
    return;
}


