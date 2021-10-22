//
//  max min stack class.cpp
//
//
//  Created by Riccardo Fontana on 02/12/16.
//
//
#include <iostream>
#include <string>
#include <exception>

#define MYDEBUG 0

using namespace std;

// Simple exception class
class E : public std::exception {
public:
    explicit E(const char * s) throw() : msg(s) { }
    const char * what() const throw() { return msg; }
private:
    E(){};    // no default constructor
    const char * msg;
};

// Exception objects
const E  e_inv_size("Invalid stack size!");
const E  e_stack_full("Stack is full!");
const E  e_stack_empty("Stack is empty!");

// simple fixed-size LIFO stack template
template <typename T>
class Stack {
private:
    static const int defaultSize = 10;
    static const int maxSize = 1000;
    int _size;
    int _top; // the top element is the last element inserted into the stack
    T * stackPtr;
public:
    explicit Stack(int s = defaultSize);
    ~Stack() {
        cout << "Calling destruct for Stack::object" << this << endl;
        delete[] stackPtr; }
    T & push( const T & );
    T & pop();
    bool isEmpty() const { return _top < 0; }
    bool isFull() const { return _top >= _size - 1; }  // ex. 9 >= 10 -1
    int top_index() const { return _top; }
    T & top_element();
    void reverse_stack(void);
    void print_stack_info(const char *);
    int size() const { return _size; }
};

// Stack<T> constructor
template <typename T>
Stack<T>::Stack ( int s ) {
    cout << "Calling construct for Stack::object" << this << endl;
    if(s > maxSize || s < 1) throw e_inv_size;
    else _size = s;
    stackPtr = new T[_size];
    _top = -1;
}

template <typename T>
T & Stack<T>::push ( const T & i ) {
    if(MYDEBUG)
       cout <<"Pushing " << i << " in " << this << endl;
    if(isFull()) throw e_stack_full;
    return stackPtr[++_top] = i;
}


template <typename T>
void Stack<T>::reverse_stack ( void ) {
    cout <<"Reversing stack " << this << endl;
    T * tmpStackPtr = new T[size()];
    int i=0;
    
    while (!isEmpty())
    {
        tmpStackPtr[i++] = top_element();
        pop();
    }
    // at this point our stackptr is empty and all the elements have been copied in a tmp array
    
    for(i=0;i < size(); i++)
        push(tmpStackPtr[i]);
    
    // now the stack will contain all the element in the reversed order
    delete[] tmpStackPtr;
    
    if(MYDEBUG)
        cout << "Top after reversing: " << top_element() << " ,size " << size() << endl;
    
    return;
}

template <typename T>
T & Stack<T>::pop () {
    
    if(MYDEBUG)
        cout << "Pop " << stackPtr[_top] << " from " << this << endl;
    
    if(isEmpty()) throw e_stack_empty;
    return stackPtr[_top--];
}


template <typename T>
void Stack<T>::print_stack_info(const char * label) {
    int i;
    
    cout << "Printing info for " << label << "...." << endl;
    
    if(isEmpty()) cout << "...stack is empty!" << endl;
    else
    {
    for(i=0; i<=_top; i++)
       cout <<stackPtr[i]<< " ";
    cout << endl;
    }
    cout << "TOP: " << top_element() << endl;
    cout << "TOP index: " << top_index() << endl;
    cout << "Size: " << size() << endl << endl;
    
    return;
}


template <typename T>
T & Stack<T>::top_element () {
    if( isEmpty()) {
        if(MYDEBUG)
           printf("stack %p empty for top(), returning ",this);
        return stackPtr[0];
        }
    return stackPtr[_top];
}

template <typename T>
class max_min_stack : public Stack<T>
{
    Stack<T> m_max;
    Stack<T> m_min;
    
public:
    
    explicit max_min_stack(int s) : Stack<T>::Stack(s){ }
    
    const T & max() {
        return m_max.top_element();}
    
    const T & min() {
        return m_min.top_element();}
    
    void print_stack_info(const char * label) {
        cout  << endl << "Printing info for MAX_MIN " << label << "...." << endl;
        cout << "MAX: " << max() << endl;
        cout << "MIN: " << min() << endl;
        Stack<T>::print_stack_info(label); }
    
    void push( T & arg) {
        if (Stack<T>::isEmpty() || arg > max()) {
            m_max.push(arg);}
        
        if (Stack<T>::isEmpty() || arg < min())
            m_min.push(arg);
        
        Stack<T>::push(arg);}
    
    T & pop() {
        
        if(Stack<T>::top_element() == max())
        {
            m_max.pop();
        }
        if(Stack<T>::top_element() == min())
        {
            m_min.pop();
        }
        
        return Stack<T>::pop(); }
    
};

/*
  The + operator has been overloaded for exercize purpose only
  The assumption here is that as C=A+B we mean append all the B elements in the A stack
  that is all B elements will be popped out first.
  (therefore the top of B will became the top of all C).
 */
template <typename T>
max_min_stack<T> operator + ( max_min_stack<T> & A,  max_min_stack<T> & B ) {
    
    cout << "Calling operator + for class max_stack<T>" << endl;
    
    int C_size = A.size() + B.size();
    if(MYDEBUG)
        cout << "C_size " << C_size << endl;
    
    max_min_stack<T> C = max_min_stack<T>( C_size);

    A.reverse_stack();
    while(!A.isEmpty())
        C.push( A.pop() );
    
    B.reverse_stack();
    while(!B.isEmpty())
        C.push( B.pop() );
    
    return C;
}


int main( int argc, char ** argv ) {
   
    try {
        
        const char * s1_label = "Stack1";
        const char * s2_label = "Stack2";
        const char * s3_label = "Stack3";
        max_min_stack<int> si(10);
        max_min_stack<int> si2(5);
        /*
        cout << s1_label << " size: " << si.size() << endl;
        cout << s1_label << " top: " << si.top_element() << endl;
        */
        
        si.print_stack_info(s1_label);
        si2.print_stack_info(s2_label);
        for ( int i : { 11, 100, 2, 3, 54, 5, 54, 9, 8, 98 } ) {
            si.push(i);
        }
        
        for ( int i : { 12, 10, 4, 5, 145 } ) {
            si2.push(i);
        }
        
        si.print_stack_info(s1_label);
        si2.print_stack_info(s2_label);
        
        /*
        cout << "si MAX after pushes: " << si.max() << endl;  // method of max_stack
        cout << "si MIN after pushes: " << si.min() << endl;  // method of max_stack
        cout << "si2 MAX after pushes: " << si2.max() << endl;  // method of max_stack
        cout << "si2 MIN after pushes: " << si2.min() << endl;  // method of max_stack
        */
        
        max_min_stack<int> si3 = si + si2;
        si3.print_stack_info(s3_label);
        
        /*
        cout << s3_label << " MAX: " << si3.max() << endl;  // method of max_stack
        cout << s3_label << " MIN: " << si3.min() << endl;  // method of max_stack
        cout << s3_label << " top index: " << si3.top_index() << endl;  // method of top
        */
        
        cout << "si is " << ( si.isFull() ? "" : "not " ) << "full" << endl;
        cout << "si is " << ( si.isEmpty() ? "" : "not " ) << "empty" << endl;
        cout << "si2 is " << ( si2.isFull() ? "" : "not " ) << "full" << endl;
        cout << "si2 is " << ( si2.isEmpty() ? "" : "not " ) << "empty" << endl;
        cout << "si3 is " << ( si3.isFull() ? "" : "not " ) << "full" << endl;
        cout << "si3 is " << ( si3.isEmpty() ? "" : "not " ) << "empty" << endl << endl;
        
        while(!si.isEmpty()) {
            cout  << si.pop() << endl;
        }
        
    } catch (E & e) {
        cout << "Stack error: " << e.what() << endl;
    }
    
    try {
        
        cout << endl << endl;
        max_min_stack<string> ss1(5);
        max_min_stack<string> ss2(6);
        const char * s1_label = "StringStack1";
        const char * s2_label = "StringStack2";
        const char * s3_label = "StringStack3";
        
        ss1.print_stack_info(s1_label);
        
        for (string s : { "one", "two", "three", "four", "bive" } ) {
            ss1.push(s);
        }
        
        for (string s : { "a_one", "z_two", "d_three", "c_four", "m_zive","e_five" } ) {
            ss2.push(s);
        }
        
        ss1.print_stack_info(s1_label);
        ss2.print_stack_info(s2_label);
        
        max_min_stack<string> ss3 = ss1 + ss2;
        ss3.print_stack_info(s3_label);
        
        //while(!ss1.isEmpty()) {
          //   ss1.pop();
            
        //ss1.print_stack_info(s1_label);
        //}
        
    } catch (E & e) {
        cout << "Stack error: " << e.what() << endl;
    }
    
    return 0;
}



