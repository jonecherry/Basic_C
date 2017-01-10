//
// Created by qsong on 2017/1/10.
//

#include "Blancebinarytree.h"
#include<iostream>
#include<stack>
#include<queue>
using namespace std;

const int LH=1; //左子树比右子树高1
const int EH=0; //左右子树一样高
const int RH=-1;//右子树比左子树高1
const int MAX_NODE_NUM=20; //结点数目上限

class AVL_Tree;

class AvlNode
{
    int data;
    int bf; //平衡因子
    AvlNode *lchild;
    AvlNode *rchild;
    friend class AVL_Tree;
};

class AVL_Tree
{
public:
    int Get_data(AvlNode *p)
    {
        return p->data;
    }

    void Create_AVl(AvlNode *&T) //建树
    {
        cout<<"输入平衡二叉树的元素,输入-1代表结束输入：";
        int num[MAX_NODE_NUM];
        int a,i=0;
        while(cin>>a && a!=-1)
        {
            num[i]=a;
            i++;
        }

        if(num[0]==-1)
        {
            cout<<"平衡树为空"<<endl;
            T=NULL;
            return;
        }

        int k=i;
        bool taller=false;
        for(i=0;i<k;i++)
            Insert_Avl(T,num[i],taller);//逐个进行插入，插入过程看下面的示意图
        cout<<"_____建树完成____"<<endl;
    }

    void L_Rotate(AvlNode *&p)
    {
        //以p为根节点的二叉排序树进行单向左旋处理
        AvlNode *rc=p->rchild;
        p->rchild=rc->lchild;
        rc->lchild=p;
        p=rc;
    }

    void R_Rotate(AvlNode *&p)
    {
        //以p为根节点的二叉排序树进行单向右旋处理
        AvlNode *lc=p->lchild;
        p->lchild=lc->rchild;
        lc->rchild=p;
        p=lc;
    }

    void Left_Balance(AvlNode *&T)
    {
        //以T为根节点的二叉排序树进行左平衡旋转处理
        AvlNode *lc,*rd;
        lc=T->lchild;
        switch(lc->bf)
        {
            case LH:
                //新结点插在T的左孩子的左子树上，做单向右旋处理
                T->bf=lc->bf=EH;
                R_Rotate(T);
                break;
            case RH:
                //新结点插在T的左孩子的右子树上，要进行双旋平衡处理（先左后右）
                rd=lc->rchild;
                switch(rd->bf)
                {
                    case LH:
                        //插在右子树的左孩子上
                        T->bf=RH;
                        lc->bf=EH;
                        break;
                    case EH:
                        T->bf=lc->bf=EH;
                        break;
                    case RH:
                        T->bf=EH;
                        lc->bf=LH;
                        break;
                }
                rd->bf=EH;
                L_Rotate(T->lchild);//先对T的左子树进行单向左旋处理
                R_Rotate(T);        //再对T进行单向右旋处理
        }
    }

    void Right_Balance(AvlNode *&T)
    {
        //以T为根节点的二叉排序树进行右平衡旋转处理
        AvlNode *rc,*ld;
        rc=T->rchild;
        switch(rc->bf)
        {
            case RH:
                //新结点插在右孩子的右子树上，进行单向左旋处理
                T->bf=rc->bf=EH;
                L_Rotate(T);
                break;
            case LH:
                //新结点插在T的右孩子的左子树上，要进行右平衡旋转处理（先右再左）
                ld=rc->lchild;
                switch(ld->bf)
                {
                    case LH:
                        T->bf=LH;
                        rc->bf=EH;
                        break;
                    case EH:
                        T->bf=rc->bf=EH;
                        break;
                    case RH:
                        T->bf=EH;
                        rc->bf=RH;
                        break;
                }
                ld->bf=EH;
                R_Rotate(T->rchild);//先对T的右子树进行单向右旋处理
                L_Rotate(T);        //再对T进行单向左旋处理
        }
    }

    bool Insert_Avl(AvlNode *&T,int num,bool &taller) //插入
    {
        //若在平衡二叉树中不存在结点值和num一样大小的结点
        //则插入值为num的新结点，并返回true
        //若因为插入而使得二叉排序树失去平衡，则做平衡旋转处理
        //taller反映树是否长高

        if(!T)
        {
            //插入新结点，树长高，taller为true
            T=new AvlNode;
            T->data=num;
            T->lchild=T->rchild=NULL;
            T->bf=EH;
            taller=true;
        }
        else
        {
            if(num==T->data)
            {
                //不重复插入
                taller=false;
                return false;
            }
            if(num<T->data) //继续在T的左子树中进行搜索
            {
                if(!Insert_Avl(T->lchild,num,taller))//插入不成功
                    return false;
                if(taller) //已插入T的左子树,且左子树长高
                {
                    switch(T->bf)
                    {
                        case LH:
                            /*—————————————————————
                            / 插入前左子树高于右子树，需要进行做平衡处理
                            / 不管是单向左旋处理，还是先左后右平衡处理
                            / 处理结果都是使得插入新结点后，树的高度不变
                            /—————————————————————*/

                            Left_Balance(T);
                            taller=false;
                            break;
                        case EH:
                            //插入前左右子树等高，现在插入新街点后，左子树比右子树高

                            T->bf=LH;
                            taller=true;
                            break;
                        case RH:
                            //插入前右子树比左子树高，现在新结点插入左子树后，树变为左右子树等高

                            T->bf=EH;
                            taller=false;
                            break;

                    }
                }
            }
            else
            {
                //num>T->data 在T的右子树中继续搜索
                if(!Insert_Avl(T->rchild,num,taller))
                    return false;
                if(taller)
                {
                    switch(T->bf)
                    {
                        case LH:
                            //插入前左子树比右子树高，现在插入T的右子树后，左右子树等高

                            T->bf=EH;
                            taller=false;
                            break;
                        case EH:
                            //插入前左右子树等高，现在插入后，右子树比左子树高

                            T->bf=RH;
                            taller=true;
                            break;

                        case RH:
                            //插入前右子树比坐子树高，插入后，排序树失去平衡，需要进行右平衡处理
                            Right_Balance(T);
                            taller=false;
                            break;

                    }
                }
            }
        }
        return true;
    }

    bool Search_Avl(AvlNode *T,int num,AvlNode *&f,AvlNode *&p) //搜索
    {
        //用p带回查找到的顶点的地址,f带回p的双亲结点
        p=T;
        while(p)
        {
            if(p->data==num)
                return true;
            if(p->data>num)
            {
                f=p;
                p=p->lchild;
            }
            else
            {
                f=p;
                p=p->rchild;
            }
        }
        return false;
    }