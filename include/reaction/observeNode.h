#ifndef REACTION_OBSERVERNODE_H
#define REACTION_OBSERVERNODE_H
#include <vector>
#include <unordered_set>
#include "reaction/concept.h"
#include "reaction/utility.h"
namespace reaction
{
    class ObserveNode
    {
    public:
        ObserveNode() = default;
        virtual ~ObserveNode() = default;

        virtual void valueChange() {
            this->notify();
        };

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


    class FieldGraph
    {
    public:
        static FieldGraph& getInstance()
        {
            static FieldGraph instance;  
            return instance;
        }

        void addObj(uint64_t id, NodePtr node)
        {
            m_fieldMap[id].insert(node);
        }

        void delObj(uint64_t id)
        {
            m_fieldMap.erase(id);
        }

        void bindField(uint64_t id, NodePtr objPtr)
        {
            if(!m_fieldMap.count(id))
            {
                return;
            }
            for (auto& node : m_fieldMap[id])
            {
                node->addObserver(objPtr.get());
            }
        }
    private:
        FieldGraph() = default;
        std::unordered_map<UniqueID, std::unordered_set<NodePtr>> m_fieldMap;
    };
}
#endif // REACTION_OBSERVERNODE_H