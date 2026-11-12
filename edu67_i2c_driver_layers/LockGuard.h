#ifndef EDU67_LOCKGUARD_H
#define EDU67_LOCKGUARD_H

/**
 * RAII lock: lock() in the constructor, unlock() in the destructor.
 * Every "return false" in the middle of a function unlocks automatically.
 */
template <class T>
class LockGuard
{
public:
    explicit LockGuard(T& rLockableP) : rLockableM(rLockableP) { rLockableM.lock(); }
    ~LockGuard() { rLockableM.unlock(); }

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;

private:
    T& rLockableM;
};

#endif // EDU67_LOCKGUARD_H
