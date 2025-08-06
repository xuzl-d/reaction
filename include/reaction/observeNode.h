#include <vector>
#include <unordered_set>
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
            (void)(..., args.getPtr()->addObserver(this));
        };

    private:
        std::vector<ObserveNode *> m_observers;
    };

    using NodePtr = std::shared_ptr<ObserveNode>;

    // ObserverGraph to manage ObserveNodes
    // This class is responsible for managing the lifecycle of ObserveNodes
    // and their relationships with each other.
    class ObserverGraph
    {
    public:
        static ObserverGraph& getInstance()
        {
            static ObserverGraph instance;  
            return instance;
        }

        void addNode(NodePtr node)
        {
            m_nodes.insert(node);
        }

        void removeNode(NodePtr node)
        {
            m_nodes.erase(node);
        }
    private:
        ObserverGraph() = default;
        std::unordered_set<NodePtr> m_nodes;
    };
}