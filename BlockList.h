/*********************************************************************************
 * BlockList.h                                                                   *
 * BlockList(double linked list of Block),                                       *
 * Block,                                                                        *
 * iterator(iterator for BlockList)                                              *
 *********************************************************************************/

#ifndef BLOCKLIST_H
#define BLOCKLIST_H

using namespace std;

template<typename T>
class BlockList
{
private:
	class Block;
	struct Node
	{
		Node* pLast = nullptr;  //指向上一个Node的指针
		Node* pNext = nullptr;  //指向下一个Node的指针
		T value;
		int blockID;
	};
		
	unsigned int _blockLength = 100;  //每个块的长度
	unsigned int _count = 0;  //元素个数
	Block* pHead = nullptr;   //指向首块的指针
	Block* pRear = nullptr;   //指向尾块的指针
	double _expandRate = 0.1;  //插入数据已满时块的增长率

public:
	class iterator;  //迭代器
	iterator begin()
	{
		return iterator(&(this->pHead->headNode()));
	}
	iterator end()
	{
		return iterator(this->pRear->rearNode().pNext);
	}

public:
	//默认构造函数，块的长度为100
	BlockList()
	{
		pHead = new Block(0, _blockLength);
		pRear = pHead;
	}
	//设定块的长度的构造函数
	BlockList(unsigned int blockLength)
	{
		this->_blockLength = blockLength;
		pHead = new Block(0, _blockLength);
		pRear = pHead;
	}
	//拷贝构造函数
	BlockList(BlockList& copy)
	{
		this->_blockLength = copy._blockLength;
		this->_expandRate = copy._blockLength;
		pHead = new Block(0, _blockLength);
		pRear = pHead;
		for (unsigned int i = 0; i < copy._count; i++)
			this->push_back(copy[i]);
	}
	//析构函数
	~BlockList()
	{
		Block* p = pHead;
		Block* pTemp;
		while (p != nullptr)
		{
			pTemp = p->pNext;
			delete p;
			p = pTemp;
		}
		pHead = nullptr;
		pRear = nullptr;
	}

	//获取BlockList中元素的个数
	unsigned int getCount() const
	{
		return this->_count;
	}
	//设置块每次增长的比率
	void setExpandRate(double expandRate)
	{
		this->_expandRate = expandRate
	}

	//在BlockList尾部追加一个元素
	void push_back(T element)
	{
		if (pRear->getEndIndex() != pRear->getLength())  //如果尾块的endIndex不为length，则直接在尾块上追加
		{			
			pRear->push_back(element);
		}
		else  //如果尾部块已满，则在尾部新增一个块，并在此块上追加
		{
			Block* pNewBlock = new Block(pRear->getID() + 1, _blockLength);
			pNewBlock->push_back(element);
			pNewBlock->pLast = pRear;
			pRear->pNext = pNewBlock;
			pRear = pNewBlock;
			pRear->headNode().pLast = &pRear->pLast->rearNode();
			pRear->pLast->rearNode().pNext = &pRear->headNode();
		}
		this->_count++;
	}
	//在首部追加一个元素
	void push_front(T element)
	{
		if (pHead->getFirstIndex() != 0)  //如果首个块的firstIndex不为0，则直接在首个块上push_front
		{
			pHead->push_front(element);
		}
		else  //否则在首部新增一个倒序块，再push_front
		{
			Block* pNewBlock = new Block(pHead->getID() - 1, _blockLength, true);
			pNewBlock->push_front(element);
			pNewBlock->pNext = pHead;
			pHead->pLast = pNewBlock;
			pHead = pNewBlock;
			pHead->rearNode().pNext = &pHead->pNext->headNode();
			pHead->pNext->headNode().pLast = &pHead->rearNode();
		}
		this->_count++;
	}
	//删除BlockList的最后一个元素
	void pop_back()
	{
		pRear->pop_back();
		if (pRear->getCount() == 0 && pRear != pHead)  //当pRear指向块的数据被全部删除且该块不是最后一个块时，删除该块
		{
			Block* pTemp = pRear->pLast;
			delete pRear;
			pRear = pTemp;
			pRear->pNext = nullptr;
			pRear->setRearNodeNext();
		}	
		this->_count--;
	}
	//删除BlockList的第一个元素
	void pop_front()
	{
		pHead->pop_front();
		if (pHead->getCount() == 0 && pRear != pHead)  //当pHead指向块的数据被全部删除且该块不是最后一个块时，删除该块
		{
			Block* pTemp = pHead->pNext;
			delete pHead;
			pHead = pTemp;
			pHead->pLast = nullptr;
			pHead->setHeadNodeLast();
		}
		this->_count--;
	}
	//删除指定位置的元素
	void deleteAtIndex(unsigned int index)
	{
		if (index >= this->_count)
			throw runtime_error("索引超出了界限");
		this->_count--;
		Block* p = pHead;
		while (p != nullptr)
		{
			if (index < p->getCount())
			{
				p->deleteAtIndex(index);
				if (p->getCount() == 0 && pHead != pRear)  //当一个块的数据全部删除时且这个块不是最后一个块时，删除该块
				{
					if (p == pHead)
					{
						pHead = p->pNext;
						pHead->setHeadNodeLast();
					}
					else if (p == pRear)
					{
						pRear = p->pLast;
						pRear->setRearNodeNext();
					}
					else
					{
						p->pLast->pNext = p->pNext;
						p->pLast->setRearNodeNext(&p->pNext->headNode());
						p->pNext->pLast = p->pLast;
						p->pNext->setHeadNodeLast(&p->pLast->rearNode());
					}
					delete p;
				}				
				return;
			}
			else
			{
				index -= p->getCount();
				p = p->pNext;
			}
		}
		throw runtime_error("发生错误");
	}
	//在指定位置插入元素
	void insert(unsigned int index, T element)
	{
		if (index >= this->_count)
			throw runtime_error("索引超出了界限");
		this->_count++;
		Block* p = pHead;
		while (p != nullptr)
		{
			if (index < p->getCount())
			{
				if (!p->isFull())
				{
					p->insert(index, element);
				}
				else
					p->insert(index, element, _expandRate);
				if (pHead != pRear)  //有超过一个块时，需调整
				{
					if (p == pHead)
					{
						p->setRearNodeNext(&p->pNext->headNode());
						p->pNext->setHeadNodeLast(&p->rearNode());
					}
					else if (p == pRear)
					{
						p->setHeadNodeLast(&p->pLast->rearNode());
						p->pLast->setRearNodeNext(&p->headNode());
					}
					else
					{
						p->setHeadNodeLast(&p->pLast->rearNode());
						p->pLast->setRearNodeNext(&p->headNode());
						p->setRearNodeNext(&p->pNext->headNode());
						p->pNext->setHeadNodeLast(&p->rearNode());
					}
				}
				return;
			}
			else
			{
				index -= p->getCount();
				p = p->pNext;
			}
		}
		throw runtime_error("发生错误");
	}
	//压缩，将所有块的长度与容量保持一致
	void compress()
	{
		Block* p = pHead;
		while (p != nullptr)
		{
			p->compress();
			if (p == pHead)  //p指向头结点
				p->headNode().pLast = nullptr;
			else
			{				
				p->headNode().pLast = &p->pLast->rearNode();
				p->pLast->rearNode().pNext = &p->headNode();
			}
			p = p->pNext;
		}
	}

	//重载[]运算符
	T& operator[](unsigned int index)
	{
		if (index >= this->_count)
			throw runtime_error("索引超出了界限");
		Block* p = pHead;
		while (p != nullptr)
		{
			if (index < p->getCount())
				return (*p)[index];
			else
			{
				index -= p->getCount();
				p = p->pNext;
			}
		}
		throw runtime_error("发生错误");
	}
	//重载赋值运算符
	BlockList& operator=(BlockList& that)
	{
		Block* p = pHead;
		Block* pTemp;
		while (p != nullptr)
		{
			pTemp = p->pNext;
			delete p;
			p = pTemp;
		}
		
		this->_blockLength = that._blockLength;
		this->_expandRate = that._blockLength;
		pHead = new Block(0, _blockLength);
		pRear = pHead;
		for (unsigned int i = 0; i < that._count; i++)
			this->push_back(that[i]);
		return *this;
	}
};

template<typename T>
class BlockList<T>::Block
{
private:
	int _id;  //块的id，从头到尾一定是增序，用于比较迭代器
	bool _isReverse = false;  //元素是否是倒序排列
	unsigned int _length = 100;  //块的长度	
	unsigned int firstIndex = 0;
	unsigned int endIndex = 0;	
	unsigned int _count = 0;  //块中元素的个数

public:
	Node* pElement = nullptr;
	Block* pLast = nullptr;  //指向上一个块的指针
	Block* pNext = nullptr;  //指向下一个块的指针

	/******************构造函数和析构函数******************/
	Block(int id, unsigned int length)
	{
		_id = id;
		this->_length = length;
		pElement = new Node[_length + 1];  //设置一个冗余位置解决最后一个元素Next的问题
		for (unsigned int i = 0; i < _length + 1; i++)
		{
			pElement[i].blockID = _id;
			pElement[i].pLast = &pElement[i - 1];
			pElement[i].pNext = &pElement[i + 1];
		}
	}
	Block(int id, unsigned int length, bool isReverse)
	{
		_id = id;
		this->_length = length;
		pElement = new Node[_length + 1];  //设置一个冗余位置解决最后一个元素Next的问题
		this->_isReverse = isReverse;
		if (this->_isReverse == true)
		{
			firstIndex = this->_length;
			endIndex = this->_length;
		}
		for (unsigned int i = 0; i < _length + 1; i++)
		{
			pElement[i].blockID = _id;
			pElement[i].pLast = &pElement[i - 1];
			pElement[i].pNext = &pElement[i + 1];
		}
	}
	~Block()  //析构函数
	{
		delete[] pElement;
		pElement = nullptr;
		pLast = nullptr;
		pNext = nullptr;		
	}

	/******************get和set函数******************/
	bool isFull() const  //判断数据块是否已满
	{
		if (this->_count < this->_length)
			return false;
		else
			return true;
	}
	bool isReverse() const  //判断元素是否是倒序排列
	{
		return _isReverse;
	}
	int getID() const  //获取数据块的id
	{
		return _id;
	}
	unsigned int getLength() const  //获取数据块的长度
	{
		return this->_length;
	}
	unsigned int getCount() const  //获取数据块中元素的个数
	{
		return this->_count;
	}
	unsigned int getFirstIndex() const  //获取数据块的开始索引
	{
		return this->firstIndex;
	}
	unsigned int getEndIndex() const  //获取数据块的结束索引
	{
		return this->endIndex;
	}
	Node& headNode()  //返回头结点的引用
	{
		return pElement[firstIndex];
	}
	Node& rearNode()  //返回尾结点的引用
	{
		return pElement[endIndex - 1];
	}
	void setHeadNodeLast()  //设置块的头元素的Last指针,无参数则设置为nullptr
	{
		this->pElement[firstIndex].pLast = nullptr;
	}
	void setHeadNodeLast(Node* p)  //设置块的头元素的Last指针为指定地址
	{
		this->pElement[firstIndex].pLast = p;
	}
	void setRearNodeNext()  //设置块的尾元素的指针,无参数则设置为下一位置
	{
		this->pElement[endIndex - 1].pNext = &pElement[endIndex];
	}
	void setRearNodeNext(Node* p)  //设置块的尾元素的指针为指定地址
	{
		this->pElement[endIndex - 1].pNext = p;
	}

	/******************其他功能函数******************/
	void push_back(T element)  //在数据块尾部追加元素，此块一定是BlockList的尾块
	{
		this->pElement[this->endIndex].value = element;
		this->endIndex++;
		this->_count++;
	}
	void push_front(T element)  //在数据块首部追加元素，此块一定是BlockList的首块
	{
		this->firstIndex--;
		this->pElement[this->firstIndex].value = element;
		this->_count++;
	}
	void pop_back()  //删除块的末尾元素，此块一定是BlockList的尾块
	{
		this->endIndex--;
		this->_count--;
	}
	void pop_front()  //删除块的第一个元素，此块一定是BlockList的首块
	{
		this->firstIndex++;
		this->_count--;
	}
	void compress()  //压缩，将块的长度与块的容量保持一致(被BlockList.compress调用)
	{
		if (this->_count < this->_length)  //只要在块的长度小于块的容量时才执行压缩
		{
			this->_length = this->_count;
			Node* pTemp = new Node[this->_length + 1];
			for (unsigned int i = 0; i < _length + 1; i++)
			{
				pTemp[i].blockID = _id;
				pTemp[i].pLast = &pTemp[i - 1];
				pTemp[i].pNext = &pTemp[i + 1];
			}
			for (unsigned int i = firstIndex; i < endIndex; i++)
				pTemp[i - firstIndex].value = pElement[i].value;
			delete[] pElement;  //释放原来pElement的空间
			pElement = pTemp;
			this->firstIndex = 0;
			this->endIndex = this->_length;
		}
	}
	void deleteAtIndex(unsigned int index)  //删除指定位置的元素
	{
		for (unsigned int i = index + firstIndex; i < endIndex - 1; i++)
			pElement[i].value = pElement[i + 1].value;		//往前删除
		this->endIndex--;
		this->_count--;
		pElement[endIndex - 1].pNext = pElement[endIndex].pNext;  //调整尾节点的pNext
	}
	void insert(unsigned int index, T element)  //在块未满的情况下,在指定位置插入元素
	{
		this->_count++;
		if (firstIndex != 0)  //如果前有空,则向前移动数据实现插入
		{
			for (unsigned int i = firstIndex; i <= firstIndex + index; i++)
				pElement[i - 1].value = pElement[i].value;
			pElement[index].value = element;
			pElement[firstIndex].pLast = &pElement[firstIndex - 1];  //调整之前首元素的pLast
			firstIndex--;
		}
		else if (endIndex != _length)  //如果尾有空,则向后移动数据实现插入
		{
			for (unsigned int i = endIndex; i > index; i--)
				pElement[i].value = pElement[i - 1].value;
			pElement[index].value = element;
			pElement[endIndex - 1].pNext = &pElement[endIndex];  //调整之前末尾元素的pNext
			endIndex++;
		}
		else
		{
			this->_count--;
			throw runtime_error("发生错误!");
		}
	}
	void insert(unsigned int index, T element, double rate)  //当块已满时,扩充块的容量,并在指定位置插入元素
	{
		if (_length < 5)
			_length = _length + 1;
		else
			_length = _length + (_length + 5) * rate;

		Node* pTemp = new Node[this->_length + 1];
		for (unsigned int i = 0; i < _length + 1; i++)
		{
			pTemp[i].blockID = _id;
			pTemp[i].pLast = &pTemp[i - 1];
			pTemp[i].pNext = &pTemp[i + 1];
		}
		this->_count++;
		if (!this->_isReverse)  //数据块是正序，则从前往后重新排列
		{
			for (unsigned int i = firstIndex; i < firstIndex + index; i++)
				pTemp[i - firstIndex].value = pElement[i].value;
			pTemp[index].value = element;
			for (unsigned int i = firstIndex + index; i < endIndex; i++)
				pTemp[i - firstIndex + 1].value = pElement[i].value;
			firstIndex = 0;
			endIndex = this->_count;
		}
		else              //数据块是倒序，则从后往前重新排列
		{
			index += firstIndex;
			for (unsigned int i = 1; i <= endIndex - index; i++)
				pTemp[_length - i].value = pElement[endIndex - i].value;			
			pTemp[_length - endIndex + index - 1].value = element;
			for (unsigned int i = endIndex - index + 1; i <= endIndex - firstIndex; i++)
				pTemp[_length - i - 1].value = pElement[endIndex - i].value;
			endIndex = _length;
			firstIndex = endIndex - _count;
		}
		delete[] pElement;  //释放原来pElement的空间
		pElement = pTemp;
	}

	T& operator[](const unsigned int &index)  //重载[]运算符
	{
		if (_isReverse == false)  //数据块是正序
			return this->pElement[index].value;
		else                      //数据块是倒序
			return this->pElement[firstIndex + index].value;
	}
};

template<typename T>
class BlockList<T>::iterator
{
private:
	Node* pNow;

public:
	typedef T value_type;  //值的类型
	typedef T* pointer;    //指针
	typedef T& reference;  //引用
	typedef int difference_type;  //两个迭代器之间的记录数
	typedef std::random_access_iterator_tag iterator_category;  //迭代器的类型是随机迭代器

	iterator(Node* p)
	{
		this->pNow = p;
	}
	iterator(const iterator& iter)  //拷贝构造函数
	{
		this->pNow = iter.pNow;
	}
	bool operator==(const iterator &that){ return this->pNow == that.pNow; }
	bool operator!=(const iterator &that){ return this->pNow != that.pNow; }
	bool operator<(const iterator &that)
	{
		if (this->pNow->blockID != that.pNow->blockID)        //若不在一个块内
			return this->pNow->blockID < that.pNow->blockID;  //比较块id
		else                                //若在一个块内
			return this->pNow < that.pNow;  //直接比较指针大小(因为一个块连续存储)
	}
	bool operator<=(const iterator &that)
	{ 
		if (this->pNow->blockID != that.pNow->blockID)        //若不在一个块内
			return this->pNow->blockID < that.pNow->blockID;  //比较块id
		else                                 //若在一个块内
			return this->pNow <= that.pNow;  //直接比较指针大小(因为一个块连续存储)
	}
	bool operator>(const iterator &that)
	{
		if (this->pNow->blockID != that.pNow->blockID)        //若不在一个块内
			return this->pNow->blockID > that.pNow->blockID;  //比较块id
		else                                //若在一个块内
			return this->pNow > that.pNow;  //直接比较指针大小(因为一个块连续存储)
	}
	bool operator>=(const iterator &that)
	{
		if (this->pNow->blockID != that.pNow->blockID)        //若不在一个块内
			return this->pNow->blockID > that.pNow->blockID;  //比较块id
		else                                 //若在一个块内
			return this->pNow >= that.pNow;  //直接比较指针大小(因为一个块连续存储)
	}
	T& operator[](int n)  //重载[]运算符
	{
		Node* pTemp = pNow;
		for (int i = 1; i <= n; i++)
			p = p->pNext;
		return p->value;
	}
	reference operator*() const  //重载*运算符
	{
		return pNow->value;
	}
	pointer operator->() const
	{
		return &(pNow->value);
	}
	iterator operator++()  //重载前缀++运算符
	{
		pNow = pNow->pNext;
		return *this;
	}
	iterator operator++(int)  //重载后缀++运算符
	{
		iterator temp = *this;
		++*this;
		return temp;
	}
	iterator operator--()  //重载前缀--运算符
	{
		pNow = pNow->pLast;
		return *this;
	}
	iterator operator--(int)  //重载后缀--运算符
	{
		iterator temp = *this;
		--*this;
		return temp;
	}
	iterator operator+=(int n)  //将迭代器向前移动n个元素
	{
		if (n < 0)
			return *this -= -n;
		for (int i = 1; i <= n; i++)
			pNow = pNow->pNext;
		return *this;
	}
	iterator operator-=(int n)  //将迭代器向后移动n个元素
	{
		if (n < 0)
			return *this += -n;
		for (int i = 1; i <= n; i++)
			pNow = pNow->pLast;
		return *this;
	}
	iterator operator+(int n)  //重载+运算符
	{
		iterator temp = *this;
		temp += n;
		return temp;
	}
	iterator operator-(int n)  //重载-运算符
	{
		iterator temp = *this;
		temp -= n;
		return temp;
	}
	difference_type operator-(const iterator &that)  //获取两个迭代器的间距
	{
		int i = 0;
		if ((*this) <= that)
		{
			iterator temp = *this;
			while (temp != that)
			{
				++temp;
				++i;
			}
			i = -i;
		}
		else
		{
			iterator temp = that;
			while (temp != *this)
			{
				++temp;
				++i;
			}			
		}
		return i;
	}
	
};

#endif