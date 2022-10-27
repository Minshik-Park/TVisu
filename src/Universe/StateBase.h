#pragma once
#include "Common/TVisu.h"
#include "Scene.h"

namespace TVisu::Universe
{
    class StateBase
    {
    public:
        StateBase(const uint32_t id);
        virtual ~StateBase();

        const uint32_t Id() const { return m_id; }

        virtual result_t Enter() = 0;
        virtual result_t Exit() = 0;

        virtual result_t Update() = 0;
        virtual result_t Render() = 0;

    protected:
        std::shared_ptr<Scene> m_spScene;

    private:
        uint32_t m_id;
    };
}
