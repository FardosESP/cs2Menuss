#pragma once

// Resolver - Task 24 (Preparation)
// TODO: Implement in future phases

struct ResolverConfig {
    bool enabled;
    bool bruteforce;
    // Add more options as needed
};

class Resolver {
public:
    Resolver() = default;
    ~Resolver() = default;

    // TODO: Implement in future phases
    void Update();
    void SetConfig(const ResolverConfig& config);

private:
    ResolverConfig config{};
};
