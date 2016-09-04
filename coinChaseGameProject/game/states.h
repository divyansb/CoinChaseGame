#ifndef STATE
#define STATE

// macro declarations to reduce typing effort
#define aware awareState::instance()
#define unaware unawareState::instance()

// The states header file provides classes that allow the enemy entity to maintain the state machine. 
// The enemy can be in one of two states: aware and unaware.
class enemy;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     STATE                           *********************///////////////////////
// The base class for all the state classes. 
class state
{
public:
	state(){}
	virtual void enter(enemy *ptr) = 0;		// called when a state is entered.
	virtual void execute(enemy *ptr) = 0;	// called in each update cycle, for the current state.
	virtual void exit(enemy *ptr) = 0;		// called when the state is changed.
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     AWARE STATE                           *********************///////////////////////
// The class defines the functionality relevant to the aware state. The enemy seeks out the character in this state.
class awareState : public state
{
public:
	void enter(enemy *ptr);
	void execute(enemy *ptr);
	void exit(enemy *ptr);
	
	// A single instance of the state is used.
	static awareState* instance();
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     UNAWARE STATE                           *********************///////////////////////
// The unaware state. The enemy wanders around the level avoiding the obstacles.
class unawareState : public state
{
public:
	void enter(enemy *ptr);
	void execute(enemy *ptr);
	void exit(enemy *ptr);

	// A single instance of the class is used.
	static unawareState* instance();
};


#endif