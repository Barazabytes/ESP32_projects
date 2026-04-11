idf_component_register(
    SRCS "events/event_bus.c" "events/event_bus_task.c"
    INCLUDE_DIRS "events/include"
    REQUIRES modules
)