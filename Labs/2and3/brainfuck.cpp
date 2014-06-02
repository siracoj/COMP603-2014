/*
= Brainfuck

== Lab 2

Write a recursive descent parser for Brainfuck corresponding to the LL(1) grammar for the language:

----
Program -> Sequence

Sequence -> Command Sequence
Sequence -> Loop Sequence
Sequence -> any other character, ignore (treat as a comment)
Sequence -> "" (empty string)

Command -> '+' | '-' | '<' | '>' | ',' | '.'

Loop -> '[' Sequence ']'
----

For each nonterminal, write a function with the name of the nonterminal.
Peek at the next character and figure out which production (rule) to apply based on the first and/or follow sets.

If the parser's working right, you should see the program spit back the source at you (including the loop brackets).

== Lab 3

Write an interpreter and compiler for Brainfuck using Visitors.

1. Compile Brainfuck to a language of your choice. Copypasta the Printer visitor class into, say, CCompiler or JavaCompiler. It should just print out equivalent C or Java or whatever source code.
2. Interpret the AST by writing a Interpreter visitor that just executes commands based on the tree structure.

== References

* http://en.wikipedia.org/wiki/Brainfuck
* http://www.cplusplus.com/reference/
* http://en.wikipedia.org/wiki/Visitor_pattern

== Running

If you have gcc:

----
g++ -o brainfuck.exe brainfuck.cpp
brainfuck.exe helloworld.bf
----
*/

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

/**
 * Primitive Brainfuck commands
 */
typedef enum { 
    INCREMENT, // +
    DECREMENT, // -
    SHIFT_LEFT, // <
    SHIFT_RIGHT, // >
    INPUT, // ,
    OUTPUT // .
} Command;

// Forward references. Silly C++!
class CommandNode;
class Loop;
class Program;

/**
 * Visits?!? Well, that'd indicate visitors!
 * A visitor is an interface that allows you to walk through a tree and do stuff.
 */
class Visitor {
    public:
        virtual void visit(const CommandNode * leaf) = 0;
        virtual void visit(const Loop * loop) = 0;
        virtual void visit(const Program * program) = 0;
};

/**
 * The Node class (like a Java abstract class) accepts visitors, but since it's pure virtual, we can't use it directly.
 */
class Node {
    public:
        virtual void accept (Visitor *v) = 0;
};

/**
 * CommandNode publicly extends Node to accept visitors.
 * CommandNode represents a leaf node with a primitive Brainfuck command in it.
 */
class CommandNode : public Node {
    public:
        Command command;
        CommandNode(char c) {
            switch(c) {
                case '+': command = INCREMENT; break;
                case '-': command = DECREMENT; break;
                case '<': command = SHIFT_LEFT; break;
                case '>': command = SHIFT_RIGHT; break;
                case ',': command = INPUT; break;
                case '.': command = OUTPUT; break;
            }
        }
        void accept (Visitor * v) {
            v->visit(this);
        }
};


/**
 * Program is the root of a Brainfuck program abstract syntax tree.
 * Because Brainfuck is so primitive, the parse tree is the abstract syntax tree.
 */

class Sequence : public Node{
	public:
		vector<Node*> children;
};

class Program : public Sequence {
    public:
        void accept (Visitor * v) {
            v->visit(this);
        }
};

/**
 * Loop publicly extends Node to accept visitors.
 * Loop represents a loop in Brainfuck.
 */

class Loop : public Sequence {
	public:
        void accept (Visitor * v) {
            v->visit(this);
        }
};
/**
 * Read in the file by recursive descent.
 * Modify as necessary and add whatever functions you need to get things done.
 */

void command(fstream & file, Sequence * Seq);
void loop(fstream & file, Sequence * Seq);
void sequence(fstream & file, Sequence * Seq);
void parse(fstream & file, Sequence * Seq);

void command(fstream & file, Sequence * Seq){
	char c;
	file >> c;
	Seq->children.push_back(new CommandNode(c));
	sequence(file, Seq);
}


void loop(fstream & file, Sequence * Seq){
	Loop * loop = new Loop();
	Seq->children.push_back(loop); //adds loop to the stack
	sequence(file, loop); //gathers commands in loop
	sequence(file, Seq); //continues with the current loop or program
}


void sequence(fstream & file, Sequence * Seq){
	char o;
	switch((char)file.peek()){
		case '+':
		case '-':
		case '<':
		case '>':
		case ',':
		case '.':
			command(file, Seq);
			break;
		case '[':
			file >> o;
			loop(file, Seq);
			break;
		case EOF:			
			break;
		case ']':
			file >> o; //next char
			break;
		default:
			file >> o; //next char
			sequence(file, Seq);
	}	
}

void parse(fstream & file, Program * program) {
	sequence(file, program);
}


	


/**
 * A printer for Brainfuck abstract syntax trees.
 * As a visitor, it will just print out the commands as is.
 * For Loops and the root Program node, it walks trough all the children.
 */
class Printer : public Visitor { //write c code
    public:
        void visit(const CommandNode * leaf) {
            switch (leaf->command) {
                case INCREMENT:   cout << "++*ptr;\n"; break;
                case DECREMENT:   cout << "--*ptr;\n"; break;
                case SHIFT_LEFT:  cout << "--ptr;\n"; break;
                case SHIFT_RIGHT: cout << "++ptr;\n"; break;
                case INPUT:       cout << "*ptr=getchar();\n"; break;
                case OUTPUT:      cout << "putchar(*ptr);\n"; break;
            }
        }
        void visit(const Loop * loop) {
	    const vector<Node*> vec = loop->children;
            cout << "while (*ptr) {\n";
            for (vector<Node*>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		cout << "\t";
                (*it)->accept(this);
            }
            cout << "}\n";
        }
        void visit(const Program * program) {
	    cout << "#include <stdio.h>\n";
	    cout << "int main(){\n";
	    cout << "char array[30000];\n";
	    cout << "char *ptr=array;\n";
	    const vector<Node*> vec = program->children;
            for (vector<Node*>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
                (*it)->accept(this);
            }
	    cout << "return 0;\n";
	    cout << "}\n";
        }
};

int main(int argc, char *argv[]) {
    fstream file;
    Program program;
    Printer printer;
    if (argc == 1) {
        cout << argv[0] << ": No input files." << endl;
    } else if (argc > 1) {

	ofstream out("out.c");
        streambuf *coutbuf = std::cout.rdbuf(); //save old buf
        cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt

        for (int i = 1; i < argc; i++) {
            file.open(argv[i], fstream::in);	    
            parse(file, & program);
            program.accept(&printer); //pass parse tree to printer
            file.close();
        }
	system("gcc -o out.exe out.c"); //compile c code
    }
}
