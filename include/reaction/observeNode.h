#include <vector>
namespace reaction
{
    class ObserveNode
    {
    public:
        ObserveNode() = default;
        virtual ~ObserveNode() = default;

        virtual void valueChange() {};

        void notify()
        {
            for (auto &observer : m_observers)
            {
                observer->valueChange();
            }
        }

        void addObserver(ObserveNode *observer)
        {
            m_observers.emplace_back(observer);
        }

        template <typename... Args>
        void updateObserver(Args &&...args)
        {
            (void)(..., args.addObserver(this));
        };

    private:
        std::vector<ObserveNode *> m_observers;
    };
}