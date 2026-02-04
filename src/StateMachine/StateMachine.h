class StateMachine
{
public:
    virtual void AddState(string name, State* state);
    virtual void ChangeState(string name);
    virtual void Update();
};