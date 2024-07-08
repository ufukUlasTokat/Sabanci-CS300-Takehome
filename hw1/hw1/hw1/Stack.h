#ifndef _STACK_H
#define _STACK_H



template <class Object>
struct ListNode{
	Object element;
	ListNode<Object> *next;

	ListNode(Object x, ListNode<Object> *n){
		element = x;
		next = n;
	}

};

template <class Object>
        class Stack
		{
		private:
			ListNode<Object> *topOfStack;

        public:

        Stack();
        Stack(const Stack & rhs);
        ~Stack();

        bool isEmpty() const;
        bool isFull() const;
        void makeEmpty();

        void pop(Object & t) ;
        void push(const Object & x);
        Object topAndPop();
		const Object & top() const;

        const Stack & operator=(const Stack & rhs);
		};

#endif