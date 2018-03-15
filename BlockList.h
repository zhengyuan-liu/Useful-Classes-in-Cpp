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
		Node* pLast = nullptr;  //ָ����һ��Node��ָ��
		Node* pNext = nullptr;  //ָ����һ��Node��ָ��
		T value;
		int blockID;
	};
		
	unsigned int _blockLength = 100;  //ÿ����ĳ���
	unsigned int _count = 0;  //Ԫ�ظ���
	Block* pHead = nullptr;   //ָ���׿��ָ��
	Block* pRear = nullptr;   //ָ��β���ָ��
	double _expandRate = 0.1;  //������������ʱ���������

public:
	class iterator;  //������
	iterator begin()
	{
		return iterator(&(this->pHead->headNode()));
	}
	iterator end()
	{
		return iterator(this->pRear->rearNode().pNext);
	}

public:
	//Ĭ�Ϲ��캯������ĳ���Ϊ100
	BlockList()
	{
		pHead = new Block(0, _blockLength);
		pRear = pHead;
	}
	//�趨��ĳ��ȵĹ��캯��
	BlockList(unsigned int blockLength)
	{
		this->_blockLength = blockLength;
		pHead = new Block(0, _blockLength);
		pRear = pHead;
	}
	//�������캯��
	BlockList(BlockList& copy)
	{
		this->_blockLength = copy._blockLength;
		this->_expandRate = copy._blockLength;
		pHead = new Block(0, _blockLength);
		pRear = pHead;
		for (unsigned int i = 0; i < copy._count; i++)
			this->push_back(copy[i]);
	}
	//��������
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

	//��ȡBlockList��Ԫ�صĸ���
	unsigned int getCount() const
	{
		return this->_count;
	}
	//���ÿ�ÿ�������ı���
	void setExpandRate(double expandRate)
	{
		this->_expandRate = expandRate
	}

	//��BlockListβ��׷��һ��Ԫ��
	void push_back(T element)
	{
		if (pRear->getEndIndex() != pRear->getLength())  //���β���endIndex��Ϊlength����ֱ����β����׷��
		{			
			pRear->push_back(element);
		}
		else  //���β��������������β������һ���飬���ڴ˿���׷��
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
	//���ײ�׷��һ��Ԫ��
	void push_front(T element)
	{
		if (pHead->getFirstIndex() != 0)  //����׸����firstIndex��Ϊ0����ֱ�����׸�����push_front
		{
			pHead->push_front(element);
		}
		else  //�������ײ�����һ������飬��push_front
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
	//ɾ��BlockList�����һ��Ԫ��
	void pop_back()
	{
		pRear->pop_back();
		if (pRear->getCount() == 0 && pRear != pHead)  //��pRearָ�������ݱ�ȫ��ɾ���Ҹÿ鲻�����һ����ʱ��ɾ���ÿ�
		{
			Block* pTemp = pRear->pLast;
			delete pRear;
			pRear = pTemp;
			pRear->pNext = nullptr;
			pRear->setRearNodeNext();
		}	
		this->_count--;
	}
	//ɾ��BlockList�ĵ�һ��Ԫ��
	void pop_front()
	{
		pHead->pop_front();
		if (pHead->getCount() == 0 && pRear != pHead)  //��pHeadָ�������ݱ�ȫ��ɾ���Ҹÿ鲻�����һ����ʱ��ɾ���ÿ�
		{
			Block* pTemp = pHead->pNext;
			delete pHead;
			pHead = pTemp;
			pHead->pLast = nullptr;
			pHead->setHeadNodeLast();
		}
		this->_count--;
	}
	//ɾ��ָ��λ�õ�Ԫ��
	void deleteAtIndex(unsigned int index)
	{
		if (index >= this->_count)
			throw runtime_error("���������˽���");
		this->_count--;
		Block* p = pHead;
		while (p != nullptr)
		{
			if (index < p->getCount())
			{
				p->deleteAtIndex(index);
				if (p->getCount() == 0 && pHead != pRear)  //��һ���������ȫ��ɾ��ʱ������鲻�����һ����ʱ��ɾ���ÿ�
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
		throw runtime_error("��������");
	}
	//��ָ��λ�ò���Ԫ��
	void insert(unsigned int index, T element)
	{
		if (index >= this->_count)
			throw runtime_error("���������˽���");
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
				if (pHead != pRear)  //�г���һ����ʱ�������
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
		throw runtime_error("��������");
	}
	//ѹ���������п�ĳ�������������һ��
	void compress()
	{
		Block* p = pHead;
		while (p != nullptr)
		{
			p->compress();
			if (p == pHead)  //pָ��ͷ���
				p->headNode().pLast = nullptr;
			else
			{				
				p->headNode().pLast = &p->pLast->rearNode();
				p->pLast->rearNode().pNext = &p->headNode();
			}
			p = p->pNext;
		}
	}

	//����[]�����
	T& operator[](unsigned int index)
	{
		if (index >= this->_count)
			throw runtime_error("���������˽���");
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
		throw runtime_error("��������");
	}
	//���ظ�ֵ�����
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
	int _id;  //���id����ͷ��βһ�����������ڱȽϵ�����
	bool _isReverse = false;  //Ԫ���Ƿ��ǵ�������
	unsigned int _length = 100;  //��ĳ���	
	unsigned int firstIndex = 0;
	unsigned int endIndex = 0;	
	unsigned int _count = 0;  //����Ԫ�صĸ���

public:
	Node* pElement = nullptr;
	Block* pLast = nullptr;  //ָ����һ�����ָ��
	Block* pNext = nullptr;  //ָ����һ�����ָ��

	/******************���캯������������******************/
	Block(int id, unsigned int length)
	{
		_id = id;
		this->_length = length;
		pElement = new Node[_length + 1];  //����һ������λ�ý�����һ��Ԫ��Next������
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
		pElement = new Node[_length + 1];  //����һ������λ�ý�����һ��Ԫ��Next������
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
	~Block()  //��������
	{
		delete[] pElement;
		pElement = nullptr;
		pLast = nullptr;
		pNext = nullptr;		
	}

	/******************get��set����******************/
	bool isFull() const  //�ж����ݿ��Ƿ�����
	{
		if (this->_count < this->_length)
			return false;
		else
			return true;
	}
	bool isReverse() const  //�ж�Ԫ���Ƿ��ǵ�������
	{
		return _isReverse;
	}
	int getID() const  //��ȡ���ݿ��id
	{
		return _id;
	}
	unsigned int getLength() const  //��ȡ���ݿ�ĳ���
	{
		return this->_length;
	}
	unsigned int getCount() const  //��ȡ���ݿ���Ԫ�صĸ���
	{
		return this->_count;
	}
	unsigned int getFirstIndex() const  //��ȡ���ݿ�Ŀ�ʼ����
	{
		return this->firstIndex;
	}
	unsigned int getEndIndex() const  //��ȡ���ݿ�Ľ�������
	{
		return this->endIndex;
	}
	Node& headNode()  //����ͷ��������
	{
		return pElement[firstIndex];
	}
	Node& rearNode()  //����β��������
	{
		return pElement[endIndex - 1];
	}
	void setHeadNodeLast()  //���ÿ��ͷԪ�ص�Lastָ��,�޲���������Ϊnullptr
	{
		this->pElement[firstIndex].pLast = nullptr;
	}
	void setHeadNodeLast(Node* p)  //���ÿ��ͷԪ�ص�Lastָ��Ϊָ����ַ
	{
		this->pElement[firstIndex].pLast = p;
	}
	void setRearNodeNext()  //���ÿ��βԪ�ص�ָ��,�޲���������Ϊ��һλ��
	{
		this->pElement[endIndex - 1].pNext = &pElement[endIndex];
	}
	void setRearNodeNext(Node* p)  //���ÿ��βԪ�ص�ָ��Ϊָ����ַ
	{
		this->pElement[endIndex - 1].pNext = p;
	}

	/******************�������ܺ���******************/
	void push_back(T element)  //�����ݿ�β��׷��Ԫ�أ��˿�һ����BlockList��β��
	{
		this->pElement[this->endIndex].value = element;
		this->endIndex++;
		this->_count++;
	}
	void push_front(T element)  //�����ݿ��ײ�׷��Ԫ�أ��˿�һ����BlockList���׿�
	{
		this->firstIndex--;
		this->pElement[this->firstIndex].value = element;
		this->_count++;
	}
	void pop_back()  //ɾ�����ĩβԪ�أ��˿�һ����BlockList��β��
	{
		this->endIndex--;
		this->_count--;
	}
	void pop_front()  //ɾ����ĵ�һ��Ԫ�أ��˿�һ����BlockList���׿�
	{
		this->firstIndex++;
		this->_count--;
	}
	void compress()  //ѹ��������ĳ���������������һ��(��BlockList.compress����)
	{
		if (this->_count < this->_length)  //ֻҪ�ڿ�ĳ���С�ڿ������ʱ��ִ��ѹ��
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
			delete[] pElement;  //�ͷ�ԭ��pElement�Ŀռ�
			pElement = pTemp;
			this->firstIndex = 0;
			this->endIndex = this->_length;
		}
	}
	void deleteAtIndex(unsigned int index)  //ɾ��ָ��λ�õ�Ԫ��
	{
		for (unsigned int i = index + firstIndex; i < endIndex - 1; i++)
			pElement[i].value = pElement[i + 1].value;		//��ǰɾ��
		this->endIndex--;
		this->_count--;
		pElement[endIndex - 1].pNext = pElement[endIndex].pNext;  //����β�ڵ��pNext
	}
	void insert(unsigned int index, T element)  //�ڿ�δ���������,��ָ��λ�ò���Ԫ��
	{
		this->_count++;
		if (firstIndex != 0)  //���ǰ�п�,����ǰ�ƶ�����ʵ�ֲ���
		{
			for (unsigned int i = firstIndex; i <= firstIndex + index; i++)
				pElement[i - 1].value = pElement[i].value;
			pElement[index].value = element;
			pElement[firstIndex].pLast = &pElement[firstIndex - 1];  //����֮ǰ��Ԫ�ص�pLast
			firstIndex--;
		}
		else if (endIndex != _length)  //���β�п�,������ƶ�����ʵ�ֲ���
		{
			for (unsigned int i = endIndex; i > index; i--)
				pElement[i].value = pElement[i - 1].value;
			pElement[index].value = element;
			pElement[endIndex - 1].pNext = &pElement[endIndex];  //����֮ǰĩβԪ�ص�pNext
			endIndex++;
		}
		else
		{
			this->_count--;
			throw runtime_error("��������!");
		}
	}
	void insert(unsigned int index, T element, double rate)  //��������ʱ,����������,����ָ��λ�ò���Ԫ��
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
		if (!this->_isReverse)  //���ݿ����������ǰ������������
		{
			for (unsigned int i = firstIndex; i < firstIndex + index; i++)
				pTemp[i - firstIndex].value = pElement[i].value;
			pTemp[index].value = element;
			for (unsigned int i = firstIndex + index; i < endIndex; i++)
				pTemp[i - firstIndex + 1].value = pElement[i].value;
			firstIndex = 0;
			endIndex = this->_count;
		}
		else              //���ݿ��ǵ�����Ӻ���ǰ��������
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
		delete[] pElement;  //�ͷ�ԭ��pElement�Ŀռ�
		pElement = pTemp;
	}

	T& operator[](const unsigned int &index)  //����[]�����
	{
		if (_isReverse == false)  //���ݿ�������
			return this->pElement[index].value;
		else                      //���ݿ��ǵ���
			return this->pElement[firstIndex + index].value;
	}
};

template<typename T>
class BlockList<T>::iterator
{
private:
	Node* pNow;

public:
	typedef T value_type;  //ֵ������
	typedef T* pointer;    //ָ��
	typedef T& reference;  //����
	typedef int difference_type;  //����������֮��ļ�¼��
	typedef std::random_access_iterator_tag iterator_category;  //�����������������������

	iterator(Node* p)
	{
		this->pNow = p;
	}
	iterator(const iterator& iter)  //�������캯��
	{
		this->pNow = iter.pNow;
	}
	bool operator==(const iterator &that){ return this->pNow == that.pNow; }
	bool operator!=(const iterator &that){ return this->pNow != that.pNow; }
	bool operator<(const iterator &that)
	{
		if (this->pNow->blockID != that.pNow->blockID)        //������һ������
			return this->pNow->blockID < that.pNow->blockID;  //�ȽϿ�id
		else                                //����һ������
			return this->pNow < that.pNow;  //ֱ�ӱȽ�ָ���С(��Ϊһ���������洢)
	}
	bool operator<=(const iterator &that)
	{ 
		if (this->pNow->blockID != that.pNow->blockID)        //������һ������
			return this->pNow->blockID < that.pNow->blockID;  //�ȽϿ�id
		else                                 //����һ������
			return this->pNow <= that.pNow;  //ֱ�ӱȽ�ָ���С(��Ϊһ���������洢)
	}
	bool operator>(const iterator &that)
	{
		if (this->pNow->blockID != that.pNow->blockID)        //������һ������
			return this->pNow->blockID > that.pNow->blockID;  //�ȽϿ�id
		else                                //����һ������
			return this->pNow > that.pNow;  //ֱ�ӱȽ�ָ���С(��Ϊһ���������洢)
	}
	bool operator>=(const iterator &that)
	{
		if (this->pNow->blockID != that.pNow->blockID)        //������һ������
			return this->pNow->blockID > that.pNow->blockID;  //�ȽϿ�id
		else                                 //����һ������
			return this->pNow >= that.pNow;  //ֱ�ӱȽ�ָ���С(��Ϊһ���������洢)
	}
	T& operator[](int n)  //����[]�����
	{
		Node* pTemp = pNow;
		for (int i = 1; i <= n; i++)
			p = p->pNext;
		return p->value;
	}
	reference operator*() const  //����*�����
	{
		return pNow->value;
	}
	pointer operator->() const
	{
		return &(pNow->value);
	}
	iterator operator++()  //����ǰ׺++�����
	{
		pNow = pNow->pNext;
		return *this;
	}
	iterator operator++(int)  //���غ�׺++�����
	{
		iterator temp = *this;
		++*this;
		return temp;
	}
	iterator operator--()  //����ǰ׺--�����
	{
		pNow = pNow->pLast;
		return *this;
	}
	iterator operator--(int)  //���غ�׺--�����
	{
		iterator temp = *this;
		--*this;
		return temp;
	}
	iterator operator+=(int n)  //����������ǰ�ƶ�n��Ԫ��
	{
		if (n < 0)
			return *this -= -n;
		for (int i = 1; i <= n; i++)
			pNow = pNow->pNext;
		return *this;
	}
	iterator operator-=(int n)  //������������ƶ�n��Ԫ��
	{
		if (n < 0)
			return *this += -n;
		for (int i = 1; i <= n; i++)
			pNow = pNow->pLast;
		return *this;
	}
	iterator operator+(int n)  //����+�����
	{
		iterator temp = *this;
		temp += n;
		return temp;
	}
	iterator operator-(int n)  //����-�����
	{
		iterator temp = *this;
		temp -= n;
		return temp;
	}
	difference_type operator-(const iterator &that)  //��ȡ�����������ļ��
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