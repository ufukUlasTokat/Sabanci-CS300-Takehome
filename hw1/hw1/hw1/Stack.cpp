#include "Stack.h"



;using namespace std;

template <class Object> //a function checks if stack is full
bool Stack<Object>::isFull() const{
	return false;
}

template <class Object> //a function checks if stack is empty
bool Stack<Object>::isEmpty() const{
      return topOfStack == NULL;
}

template <class Object> //a function returns top element
const Object & Stack<Object>::top() const{
	if (isEmpty())
        return topOfStack->element;
}


template <class Object> //a function returns top element and delete it
void Stack<Object>::pop(Object & t){
	ListNode<Object> *temp ;
	if (!isEmpty()){
		t = topOfStack->element;                  
		temp = topOfStack->next;
		delete topOfStack;
		topOfStack = temp;
	}
}

template <class Object>//a function put a new element to stack
void Stack<Object>::push(const Object & x){
        topOfStack = new ListNode<Object>(x, topOfStack);
}

template <class Object> 
Object Stack<Object>::topAndPop(){
        Object topItem = top();
		Object temp;
        pop(temp);
        return topItem;
}

template <class Object>//makes stack empty
void Stack<Object>::makeEmpty(){
    while (!isEmpty()){
        Object temp;
		pop(temp);
	}
}

template <class Object>//an operator makes two stack equal
const Stack<Object> & Stack<Object>::
operator=( const Stack<Object> & rhs ){
        if (this != &rhs)
        {
            makeEmpty( );
            if (rhs.isEmpty())
                return *this;

            ListNode *rptr = rhs.topOfStack;
            ListNode *ptr  = new ListNode(rptr->element);
            topOfStack = ptr;

            for (rptr = rptr->next; rptr != NULL; rptr = rptr->next)
                    ptr = ptr->next = new ListNode(rptr->element);
    }
    return *this;
}

template <class Object>//parameterized constructer
Stack<Object>::Stack(const Stack<Object> & rhs){
        topOfStack = NULL;
        *this = rhs;  
}

template <class Object>//destructer
Stack<Object>::~Stack(){
        makeEmpty();
}

template <class Object>//default constructer
Stack<Object>::Stack( )
{
    topOfStack = NULL;
}

