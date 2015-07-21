#ifndef MAPPED_ELEMENTS_H_
#define MAPPED_ELEMENTS_H_

#include <vector>
#include <list>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "Data.h"

namespace maxcompilersim {

class MappedElement; // forward

class MappedElementObserver {
protected:
	std::list<MappedElement*> m_observedElements;

	friend class MappedElement;

	MappedElementObserver(): m_observedElements() {}
	void AttachMappedElement(MappedElement *elem);
	void DetachMappedElement(MappedElement *elem);

public:
	virtual ~MappedElementObserver();
	virtual void mappedElementChanged(const MappedElement &changedElement) {};
};


class MappedElement {
private:
	std::list<MappedElementObserver*> m_observers;

protected:
	uint32_t m_address;

	friend class MappedElementObserver;

	void AttachObserver(MappedElementObserver* obs) {m_observers.push_back(obs);}
	void DetachObserver(MappedElementObserver* obs) {m_observers.remove(obs);}
	void NotifyObservers();
public:
	MappedElement() : m_address(~0U), m_observers() {}
	virtual ~MappedElement();

	virtual uint64_t get(uint32_t address, bool mec) const = 0;
	virtual void set(uint32_t address, uint64_t value, uint mask, bool mec) = 0;
	virtual void assignAddress(uint32_t address);
	uint32_t getAddress() {return m_address;}
};
typedef boost::shared_ptr<MappedElement> MappedElementPtr;

class MappedRegister : public MappedElement {
private:
	Data m_data;
	const uint m_bytes;

public:
	MappedRegister(const Data init_value);

	virtual ~MappedRegister() {}

	virtual const Data &getData() const;
	virtual void setData(const Data &value);
	virtual uint64_t get(uint32_t address, bool mec) const;
	virtual void set(uint32_t address, uint64_t value, uint mask, bool mec);
	virtual void assignAddress(uint32_t address);
};

typedef boost::shared_ptr<MappedRegister> MappedRegisterPtr;

/*
 * Something on the mapped memory bus.
 */
class AbstractMappedMemory : public MappedElement {
protected:
	const uint m_bit_width;
	const uint m_depth;

public:
	AbstractMappedMemory(uint bit_width, uint depth);

	virtual ~AbstractMappedMemory() {}

	virtual const Data &getData(uint entry) const = 0;
	virtual void setData(uint entry, const Data &value) = 0;

	virtual const uint getDepth() const;
	virtual const uint getWidth() const;

	virtual void assignAddress(uint32_t address);
};
typedef boost::shared_ptr<AbstractMappedMemory> AbstractMappedMemoryPtr;

/*
 * An ordinary mapped memory.
 */
class MappedMemory : public AbstractMappedMemory {
private:
	std::vector<Data> m_data;

public:
	MappedMemory(uint bit_width, uint depth);

	virtual ~MappedMemory() {}

	virtual const Data &getData(uint entry) const;
	virtual void setData(uint entry, const Data &value);

	virtual uint64_t get(uint32_t address, bool mec) const;
	virtual void set(uint32_t address, uint64_t value, uint mask, bool mec);
};
typedef boost::shared_ptr<MappedMemory> MappedMemoryPtr;

} // namespace maxcompilersim

#endif // MAPPED_ELEMENTS_H_
