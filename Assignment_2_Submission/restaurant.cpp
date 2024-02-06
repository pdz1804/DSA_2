#include "main.h"

static int MAXSIZE = 0;

// --------------------------------------------------------------------------------------------------------------------------------
class HuffmanNode
{
private:
	int getHeightRecursive_HuffmanNode(HuffmanNode *root)
	{
		if (!root)
			return 0;
		int left_height = this->getHeightRecursive_HuffmanNode(root->left);
		int right_height = this->getHeightRecursive_HuffmanNode(root->right);

		return ((left_height > right_height) ? left_height : right_height) + 1;
	}

public:
	int freq;
	int cval;
	char c;
	HuffmanNode *left, *right;

	HuffmanNode(char c_input, int cval_input, int f_input, HuffmanNode *left_input = nullptr, HuffmanNode *right_input = nullptr)
		: freq(f_input), cval(cval_input), c(c_input), left(left_input), right(right_input)
	{
	}

	HuffmanNode(HuffmanNode *left_input = nullptr, HuffmanNode *right_input = nullptr)
	{
		this->left = left_input;
		this->right = right_input;
	}

	~HuffmanNode()
	{
		if (left)
			delete left;
		if (right)
			delete right;
	}

	int HuffmanNode_Get_Balance_Factor()
	{
		int left_height = getHeightRecursive_HuffmanNode(left);
		int right_height = getHeightRecursive_HuffmanNode(right);

		return left_height - right_height;
	}

	bool HuffmanNode_isLeaf()
	{
		return (left == nullptr) && (right == nullptr);
	}
};

struct CmpHuffmanNode
{
	bool operator()(HuffmanNode *a, HuffmanNode *b)
	{
		return (a->freq > b->freq) || (a->freq == b->freq && a->cval > b->cval);
	}
};

class HuffmanTree
{
private:
	// --------------------------------------------------------------------------------------------------------------------------------
	// THESE FUNCTIONS WILL BE USED FOR BALANCE THE HUFFMAN TREE
	// NOT UPDATE CVAL AFTER ROTATE YET
	// --------------------------------------------------------------------------------------------------------------------------------
	HuffmanNode *rotate_left(HuffmanNode *node)
	{
		HuffmanNode *tmp = node->right; // subright tree
		node->right = tmp->left;
		tmp->left = node;

		// Update cval to push back to the end of the pq nodes which has same freq
		tmp->cval = char_incre;
		char_incre++;
		return tmp;
	}

	HuffmanNode *rotate_right(HuffmanNode *node)
	{
		HuffmanNode *tmp = node->left; // subleft tree
		node->left = tmp->right;
		tmp->right = node;

		// Update cval to push back to the end of the pq nodes which has same freq
		tmp->cval = char_incre;
		char_incre++;
		return tmp;
	}

	// --------------------------------------------------------------------------------------------------------------------------------
	// traverse huffman tree and store huffman node
	void HuffmanTree_Help_Encode(HuffmanNode *node, string str,
								 unordered_map<char, string> &huffman_code)
	{
		// If nullptr
		if (!node)
			return;

		// If leaf
		if (!node->left && !node->right)
			huffman_code[node->c] = str;

		HuffmanTree_Help_Encode(node->left, str + "0", huffman_code);
		HuffmanTree_Help_Encode(node->right, str + "1", huffman_code);
	}
	// --------------------------------------------------------------------------------------------------------------------------------
public:
	static int char_incre;
	HuffmanNode *root;
	unordered_map<char, string> huffmanCode;

	HuffmanTree()
	{
		this->root = nullptr;
	}

	HuffmanTree(HuffmanNode *root_input)
	{
		this->root = root_input;
	}

	~HuffmanTree()
	{
		if (root)
			delete root;
		this->root = nullptr;
	}

	// ----------------------------------------------------------------
	// ADDING TO HELP BALANCE THE TREE
	// ----------------------------------------------------------------
	HuffmanNode *HuffmanTree_Help_Balance(HuffmanNode *check_Balance)
	{
		int BF = check_Balance->HuffmanNode_Get_Balance_Factor();

		// ADDING FOR CHECKING
		if (BF < 2 && BF > -2)
			return check_Balance;

		// OLD VERSION HERE USE WHILE, NOW NEED UPDATE ONLY NEED 1 ITERATION
		if (BF <= -2 || BF >= 2)
		{
			// LEFT LEFT
			if (BF >= 2 && check_Balance->left->HuffmanNode_Get_Balance_Factor() >= 0)
				check_Balance = rotate_right(check_Balance);
			// RIGHT RIGHT
			else if (BF <= -2 && check_Balance->right->HuffmanNode_Get_Balance_Factor() <= 0)
				check_Balance = rotate_left(check_Balance);
			// LEFT RIGHT
			else if (BF >= 2 && check_Balance->left->HuffmanNode_Get_Balance_Factor() < 0)
			{
				check_Balance->left = rotate_left(check_Balance->left);
				check_Balance = rotate_right(check_Balance);
			}
			// RIGHT LEFT
			else if (BF <= -2 && check_Balance->right->HuffmanNode_Get_Balance_Factor() > 0)
			{
				check_Balance->right = rotate_right(check_Balance->right);
				check_Balance = rotate_left(check_Balance);
			}

			BF = check_Balance->HuffmanNode_Get_Balance_Factor();
		}

		return check_Balance;
	}

	HuffmanNode *HuffmanTree_Help_Recur(HuffmanNode *check_Balance, int &count_swap, bool &hasRotate)
	{
		if (!check_Balance)
			return nullptr;

		if (count_swap == 3 || hasRotate)
			return check_Balance;

		HuffmanNode *check_temp = nullptr;
		if (abs(check_Balance->HuffmanNode_Get_Balance_Factor()) > 1 && count_swap < 3)
		{
			check_temp = HuffmanTree_Help_Balance(check_Balance);
			if (check_Balance != check_temp)
			{
				check_Balance = check_temp;
				check_temp = nullptr;
				hasRotate = true; // added
				count_swap++;

				return check_Balance;
			}
		}

		// check_Balance = HuffmanTree_Help_Balance(check_Balance);
		check_Balance->left = HuffmanTree_Help_Recur(check_Balance->left, count_swap, hasRotate);
		check_Balance->right = HuffmanTree_Help_Recur(check_Balance->right, count_swap, hasRotate);

		return check_Balance;
	}

	HuffmanNode *HuffmanTree_Recur_Balance_Root(HuffmanNode *check_Balance, int &count_swap)
	{
		bool hasRotate = false;

		while (true)
		{
			check_Balance = HuffmanTree_Help_Recur(check_Balance, count_swap, hasRotate);
			if (!hasRotate || count_swap == 3)
			{
				break;
			}
			else
			{
				hasRotate = false;
			}
		}

		return check_Balance;
	}

	void HuffmanTree_Build_Tree(vector<pair<char, int>> &vec_for_build_after_merge)
	{
		// USE THIS TO FORCE THE NEW NODE OF HUFFMAN NODE TO STAND
		// AT THE END OF THE LIST OF EQUAL FREQ NODES
		priority_queue<HuffmanNode *, vector<HuffmanNode *>, CmpHuffmanNode> pq;

		// ----------------------------------------------------------------
		// FUNCTION FOR ADDING THE INITIAL NODES OF HUFFMAN TREE
		// ----------------------------------------------------------------
		for (auto p : vec_for_build_after_merge)
		{
			// A -> Z: 65 -> 90; a -> z: 97 -> 122
			HuffmanNode *newNode = nullptr;
			if (int(p.first) >= 65 && int(p.first) <= 90)
				newNode = new HuffmanNode(p.first, int(p.first) + 100, p.second); // assign cval 100 + int(char)
			if (int(p.first) >= 97 && int(p.first) <= 122)
				newNode = new HuffmanNode(p.first, int(p.first), p.second); // assign cval int(char)
			pq.push(newNode);
		}

		// int temp_increase_length = len_for_incre;
		while (pq.size() != 1)
		{
			HuffmanNode *left = pq.top();
			pq.pop();

			HuffmanNode *right = pq.top();
			pq.pop();

			int sum_freq = left->freq + right->freq;

			HuffmanNode *newNode = new HuffmanNode('\0', char_incre++, sum_freq, left, right);

			int count_swap = 0;
			newNode = HuffmanTree_Recur_Balance_Root(newNode, count_swap);

			pq.push(newNode);
		}

		this->root = pq.top();

		pq.pop();
	}

	// --------------------------------------------------------------------------------------------------------------------------------
	// HELP ENCODED STRING
	// --------------------------------------------------------------------------------------------------------------------------------
	// traverse huffman tree and store huffman node
	void HuffmanTree_Encode(unordered_map<char, string> &huffman_code)
	{
		HuffmanTree_Help_Encode(root, "", huffman_code);
		this->huffmanCode = huffman_code;
	}

	// PRINT TOP -> BOTTOM, LEFT -> RIGHT
	void HuffmanTree_Help_HAND(HuffmanNode *node, string &output)
	{
		if (!node)
			return;

		HuffmanTree_Help_HAND(node->left, output);

		if (node->HuffmanNode_isLeaf())
		{
			output = output + node->c + "\n";
		}
		else
		{
			output = output + to_string(node->freq) + "\n";
		}

		HuffmanTree_Help_HAND(node->right, output);
	}

	string HuffmanTree_Print_HAND(HuffmanNode *node)
	{
		string output = "";
		HuffmanTree_Help_HAND(node, output);
		return output;
	}
};

int HuffmanTree::char_incre = 200;
// -----------------------------------------------------------------
class Customer
{
public:
	string print_hand;
	int result;
	int ID;

	Customer(string print_hand_input, int result_input = 0)
	{
		this->print_hand = print_hand_input;
		this->result = result_input;
		this->ID = result_input % MAXSIZE + 1;
	}

	~Customer() {}
};
// -----------------------------------------------------------------
string global_name_for_hand = "";

// MAIN FUNCTION FOR CALCULATING PERMUTATIONS OF BST TREE
int Main_Help_Count_BST(vector<int> currentNums, vector<vector<int>> combinations, const int &MOD)
{
	if (int(currentNums.size()) <= 1)
		return 1;

	vector<int> leftSubtree, rightSubtree;

	// Partition
	for (int i = 1; i < int(currentNums.size()); i++)
	{
		if (currentNums[i] >= currentNums[0])
			rightSubtree.push_back(currentNums[i]);
		else
			leftSubtree.push_back(currentNums[i]);
	}

	int left_size = int(leftSubtree.size()), right_size = int(rightSubtree.size());
	int left_count = Main_Help_Count_BST(leftSubtree, combinations, MOD), right_count = Main_Help_Count_BST(rightSubtree, combinations, MOD);

	// Calculate the number of BSTs for current partition
	return int(static_cast<long long>(combinations[left_size + right_size][left_size]) * left_count % MOD * right_count % MOD);
}

// NOTE CHANGE TOMORROW
int Main_Count_Permutations(vector<int> &nums)
{
	int n = int(nums.size());
	const int MOD = MAXSIZE;

	vector<vector<int>> combinations(n, vector<int>(n, 0));

	combinations[0][0] = 1;

	for (int i = 1; i < n; i++)
	{
		// Pascal Formula: C_{i,j} = C_{i,j-1} + C_{i-1,j-1}
		combinations[i][0] = 1;
		for (int j = 1; j <= i; j++)
			combinations[i][j] = (combinations[i - 1][j] + combinations[i - 1][j - 1]) % MOD;
	}

	return (Main_Help_Count_BST(nums, combinations, MOD) + MOD) % MOD;
}

// -----------------------------------------------------------------
class NodeBST
{
public:
	Customer *customer;
	NodeBST *left;
	NodeBST *right;

	NodeBST()
	{
		this->customer = nullptr;
		this->left = nullptr;
		this->right = nullptr;
	}

	NodeBST(Customer *cus = nullptr, NodeBST *left_input = nullptr, NodeBST *right_input = nullptr)
	{
		this->customer = cus;
		this->left = left_input;
		this->right = right_input;
	}

	~NodeBST()
	{
		if (left)
		{
			delete left;
			left = nullptr;
		}

		if (right)
		{
			delete right;
			right = nullptr;
		}

		if (customer)
		{
			delete customer;
			customer = nullptr;
		}
	}
};
// -----------------------------------------------------------------
// In Restaurant G, an area is considered as a BST Tree
class BST_Tree
{
private:
	/*
	Inside this function, we just "cut" the node that
	contains the Customer cus_del out of the tree.
	The delete state would be inside the BST Tree KOKUSEN function
	*/
	NodeBST *BST_Tree_Main_Delete(NodeBST *node, Customer *cus_del)
	{
		if (BST_Tree_isEmpty())
			return node;

		if (node->customer->result > cus_del->result)
		{
			node->left = BST_Tree_Main_Delete(node->left, cus_del);
			return node;
		}
		// BECAUSE TREE ACCEPT DUPLICATES, SO WHENEVER WE FIND THE CUSTOMER RESULT = DEL_NODE RESULT, WE DELETE IT.
		else if (node->customer->result < cus_del->result)
		{
			node->right = BST_Tree_Main_Delete(node->right, cus_del);
			return node;
		}

		if (!node->left)
		{
			NodeBST *temp = node->right;
			node->right = nullptr;

			node = nullptr;
			return temp;
		}
		else if (!node->right)
		{
			NodeBST *temp = node->left;
			node->left = nullptr;

			node = nullptr;
			return temp;
		}
		// If both children exist
		else
		{
			NodeBST *succParent = node;

			// Find successor
			NodeBST *succ = node->right;
			while (succ->left)
			{
				succParent = succ;
				succ = succ->left;
			}
			if (succParent != node)
				succParent->left = succ->right;
			else
				succParent->right = succ->right;

			// swap customer between the 2 nodes, links stay the same
			Customer *temp = node->customer;
			node->customer = succ->customer;
			succ->customer = temp;
			succ->left = nullptr;
			succ->right = nullptr;

			succ = nullptr;

			return node;
		}
	}

	void BST_Tree_Help_Delete(Customer *value)
	{
		// TODO
		if (BST_Tree_isEmpty())
			return;

		if (!value)
			return;
		root = BST_Tree_Main_Delete(root, value);
	}

	// -----------------------------------------------------------------
	// NOT DONE YET
	// -----------------------------------------------------------------
	void BST_Tree_Help_Output_Vector(NodeBST *node, vector<int> &vec)
	{
		if (!node)
			return;
		vec.push_back(node->customer->result);
		BST_Tree_Help_Output_Vector(node->left, vec);
		BST_Tree_Help_Output_Vector(node->right, vec);
	}

	vector<int> BST_Tree_Output_Vector(NodeBST *node)
	{
		vector<int> vec;
		BST_Tree_Help_Output_Vector(node, vec);
		return vec;
	}

	int BST_Tree_Help_Count_Permutation()
	{
		vector<int> nums = BST_Tree_Output_Vector(this->root);
		return Main_Count_Permutations(nums);
	}

	// HELPER FUNCTION FOR PRINTING TREE
	void help_print_inorder(NodeBST *node)
	{
		if (!node)
			return;
		help_print_inorder(node->left);

		printf("%d\n", node->customer->result); // test printf
		help_print_inorder(node->right);
	}

public:
	NodeBST *root;
	vector<NodeBST *> queue_of_customers; // Dùng queue do KOKUSEN phải xóa FIFO

	BST_Tree()
	{
		this->root = nullptr;
	}

	BST_Tree(Customer *customer)
	{
		this->root = new NodeBST(customer);
		queue_of_customers.push_back(root);
	}

	~BST_Tree()
	{
		while (!queue_of_customers.empty())
		{
			NodeBST *temp = queue_of_customers.front();
			queue_of_customers.erase(queue_of_customers.begin());
			temp->left = nullptr;
			temp->right = nullptr;
			delete temp; // delete NODEBST
		}

		this->root = nullptr;
	}

	bool BST_Tree_isEmpty()
	{
		return root == nullptr;
	}

	// Helping functions
	void BST_Tree_Insert(Customer *value)
	{
		// TODO
		if (BST_Tree_isEmpty())
		{
			root = new NodeBST(value);
			queue_of_customers.push_back(root);
			return;
		}

		NodeBST *r = root;
		while (true)
		{
			if (r->customer->result > value->result)
			{
				if (!r->left)
				{
					NodeBST *newNode = new NodeBST(value);
					r->left = newNode;
					queue_of_customers.push_back(newNode);
					break;
				}
				r = r->left;
			}
			else
			{
				if (!r->right)
				{
					NodeBST *newNode = new NodeBST(value);
					r->right = newNode;
					queue_of_customers.push_back(newNode);
					break;
				}
				r = r->right;
			}
		}
	}

	// -----------------------------------------------------------------
	// KOKUSEN FUNCTION
	// Not REPLACE MOD in HELPING COUNT OF KOKUSEN
	// -----------------------------------------------------------------
	void BST_Tree_KOKUSEN()
	{
		if (!root)
			return;

		// CALCULATE NUMBER OF PERMUTATION
		int size = int(queue_of_customers.size());
		int Y = BST_Tree_Help_Count_Permutation();
		Y = Y % MAXSIZE;

		Y = (Y <= size) ? (Y) : (size);

		for (int i = 0; i < Y; i++)
		{
			NodeBST *nodeForDelete = queue_of_customers.front();
			Customer *customerForDelete;

			if (nodeForDelete == nullptr || nodeForDelete->customer == nullptr)
				break;
			else
				customerForDelete = nodeForDelete->customer;

			BST_Tree_Help_Delete(customerForDelete);

			// If come here, finish the CUT THE DELETE NODE OUT OF THE TREE
			// Nhưng mà bởi vì bên trên chỉ CUT ra khỏi TREE bằng cách SWAP Customer của 2 thằng
			// Xuống đây ta phải SWAP 2 BST_TREE trong queue đã.
			int cur_size = int(queue_of_customers.size());

			if (cur_size == 1)
			{
				NodeBST *now_del = queue_of_customers[0];
				queue_of_customers.erase(queue_of_customers.begin());

				delete now_del; // delete NODEBST
				root = nullptr;
			}
			else
			{
				// Swap the 2 BST first
				// Because in the delete Node function, we swap the 2 customer
				// So here, in the queue, we need to find them, swap the position
				// so that the swaped customer is in its right position.
				int swap_1 = -1, swap_2 = -1;
				for (int j = 0; j < cur_size; j++)
				{
					if (queue_of_customers[j]->customer == customerForDelete)
						swap_1 = j;
					if (queue_of_customers[j] == nodeForDelete)
						swap_2 = j;
				}

				NodeBST *temp_swap = queue_of_customers[swap_1];
				queue_of_customers[swap_1] = queue_of_customers[swap_2];
				queue_of_customers[swap_2] = temp_swap;

				// Here we just find the node that we going to delete and delete it
				for (int j = 0; j < cur_size; ++j)
				{
					if (queue_of_customers[j]->customer == customerForDelete)
					{
						NodeBST *now_del = queue_of_customers[j];
						queue_of_customers.erase(queue_of_customers.begin() + j);

						delete now_del;

						break;
					}
				}
			}
		}
	}

	// HELPER FUNCTION FOR PRINTING TREE
	void BST_Tree_Print_Inorder()
	{
		if (!root)
			return;
		help_print_inorder(this->root);
	}
};
// -----------------------------------------------------------------
// MIN HEAP
class NodeHeap
{
public:
	int label_node;
	int num_customer_in_node;
	vector<Customer *> queue_node_heap;

	NodeHeap(int label = 0)
	{
		this->label_node = label;
		this->num_customer_in_node = 0;
	}

	~NodeHeap()
	{
		int size_temp = int(queue_node_heap.size());
		for (int i = 0; i < size_temp; i++)
		{
			Customer *c = queue_node_heap[0];
			queue_node_heap.erase(queue_node_heap.begin());

			delete c;
		}
		this->num_customer_in_node = 0;
	}

	void NodeHeap_Insert_Customer(Customer *cus)
	{
		this->num_customer_in_node += 1;
		queue_node_heap.push_back(cus);
	}

	void NodeHeap_Delete_All_FIFO()
	{
		if (queue_node_heap.size() == 0)
			return;

		for (int i = 0; i < num_customer_in_node; i++)
		{
			Customer *del_cus = queue_node_heap[0];
			queue_node_heap.erase(queue_node_heap.begin());

			printf("%d-%d\n", del_cus->result, del_cus->ID); // printf check

			delete del_cus;
		}

		this->num_customer_in_node = 0;
	}

	void NodeHeap_Delete_NUM_Cus_FIFO(int num)
	{
		if (queue_node_heap.size() == 0)
			return;

		for (int i = 0; i < num; i++)
		{
			Customer *del_cus = queue_node_heap[0];
			queue_node_heap.erase(queue_node_heap.begin());

			printf("%d-%d\n", del_cus->result, del_cus->ID); // printf check

			delete del_cus;
		}
		this->num_customer_in_node -= num;
	}

	void NodeHeap_CLEAVE(int num)
	{
		if (queue_node_heap.size() == 0)
			return;

		if (num > int(queue_node_heap.size()))
			num = int(queue_node_heap.size());

		for (int i = int(queue_node_heap.size()) - 1; i >= int(queue_node_heap.size()) - num; i--)
			printf("%d-%d\n", queue_node_heap[i]->ID, queue_node_heap[i]->result); // prinf check
	}
};
// -----------------------------------------------------------------
class HEAP
{
private:
	// COMPARE NODE1 < NODE2
	bool HEAP_Reheap_Help_Compare(NodeHeap *&node1, NodeHeap *&node2)
	{
		if (!node1)
			return false;
		if (!node2)
			return true;
		return node1->num_customer_in_node < node2->num_customer_in_node;
	}

	/* ==================== reheapUp ====================
	Reestablishes heap by moving data in child up to
	correct location heap array.
	*/
	// IF NUMBER OF NODES OF CHILD IS LESS THAN THEIR PARENT -> REHEAP UP
	void HEAP_Reheap_Up(int childLoc)
	{
		int parent;
		NodeHeap *hold;
		// If invalid index of childLoc
		if (childLoc < 0)
			return;

		// If not at root of heap (index 0)
		if (childLoc)
		{
			parent = (childLoc - 1) / 2;
			// ary[childLoc] < ary[parent]
			if (HEAP_Reheap_Help_Compare(heap_array_of_node[childLoc], heap_array_of_node[parent]))
			{
				hold = heap_array_of_node[parent];
				heap_array_of_node[parent] = heap_array_of_node[childLoc];
				heap_array_of_node[childLoc] = hold;
				HEAP_Reheap_Up(parent);
			}
		}
		return;
	}

	/* =================== reheapDown ===================
	Reestablishes heap by moving data in root down to its
	correct location in the heap.
	*/
	void HEAP_Reheap_Down(int root)
	{
		NodeHeap *hold;
		NodeHeap *leftData;
		NodeHeap *rightData;
		int smallLoc;

		if (root < 0)
			return;

		// Left subtree
		if (root * 2 + 1 <= idx_last_node)
		{
			leftData = heap_array_of_node[root * 2 + 1];
			// Right subtree
			if ((root * 2 + 2) <= idx_last_node)
				rightData = heap_array_of_node[root * 2 + 2];
			else
				rightData = nullptr;

			// MIN HEAP SO NEED TO CHOOSE THE SMALLER ONE
			// Determine which child is larger, choose the larger node
			if (!rightData || HEAP_Reheap_Help_Compare(leftData, rightData) || (leftData->num_customer_in_node == rightData->num_customer_in_node && HEAP_Heap_Last_Used_Find(leftData->label_node) < HEAP_Heap_Last_Used_Find(rightData->label_node)))
				smallLoc = root * 2 + 1;
			else
				smallLoc = root * 2 + 2;

			// Test if root >= smaller subtree
			if (HEAP_Reheap_Help_Compare(heap_array_of_node[smallLoc], heap_array_of_node[root]) || ((heap_array_of_node[smallLoc]->num_customer_in_node == heap_array_of_node[root]->num_customer_in_node) && (HEAP_Heap_Last_Used_Find(heap_array_of_node[smallLoc]->label_node) < HEAP_Heap_Last_Used_Find(heap_array_of_node[root]->label_node)))) // NOT(root < smallLoc) == root >= smallLoc
			{
				// swap root AND smaller subtree
				hold = heap_array_of_node[root];						 // hold root
				heap_array_of_node[root] = heap_array_of_node[smallLoc]; // put smaller value to the root
				heap_array_of_node[smallLoc] = hold;					 // put the root to the smaller value position
				HEAP_Reheap_Down(smallLoc);
			}
		}
		return;
	}

	// HELP PRINT IN CLEAVE
	void help_CLEAVE(int root, int NUM)
	{
		if (root > idx_last_node)
			return;
		if (root >= 0 && root <= idx_last_node)
			heap_array_of_node[root]->NodeHeap_CLEAVE(NUM);

		help_CLEAVE(2 * root + 1, NUM);
		help_CLEAVE(2 * root + 2, NUM);
	}

public:
	// num_customer_in_node OF NODEHEAP is considered as the VALUE OF THE NODE IN HEAP
	vector<NodeHeap *> heap_array_of_node;
	vector<int> heap_last_used; // lastly used -> recently (index 0 -> MAXSIZE - 1), a node recently used was put at last

	int idx_last_node; // index of las_t node in HEAP
	int curr_size;	   // current number of nodes in the heap_array_of_node

	HEAP()
	{
		this->curr_size = 0;
		this->idx_last_node = -1;
	}

	~HEAP()
	{
		// -----------------------------------------------------------------
		// ERASE FROM THE heap_array_of_node vector
		// -----------------------------------------------------------------
		int temp_size = int(heap_array_of_node.size());

		for (int i = 0; i < temp_size; i++)
		{
			NodeHeap *temp = heap_array_of_node[0];
			heap_array_of_node.erase(heap_array_of_node.begin());
			delete temp; // delete NODEHEAP
		}
		heap_last_used.clear();
	}

	// -----------------------------------------------------------------
	// HEAP ARY FIND: return the index if appear, otherwise return -1
	// -----------------------------------------------------------------
	int HEAP_Heap_Array_Find(int label)
	{
		int size_temp = int(heap_array_of_node.size());
		for (int v = 0; v < size_temp; v++)
		{
			if (heap_array_of_node[v]->label_node == label)
				return v;
		}
		return -1;
	}

	// HEAP LAST USED FIND: return the index if appear, otherwise return -1
	int HEAP_Heap_Last_Used_Find(int label)
	{
		int size_temp = int(heap_last_used.size());
		for (int v = 0; v < size_temp; v++)
		{
			if (heap_last_used[v] == label)
				return v;
		}
		return -1;
	}
	// -----------------------------------------------------------------
	//  ================== Heap Insert ===================
	void HEAP_Insert_Customer(Customer *cus)
	{
		int enter_id = cus->ID;
		int idx_enter_node = HEAP_Heap_Last_Used_Find(enter_id);

		NodeHeap *enter_node = nullptr;

		// NODE that Customer enter IS NOT USED YET IN HEAP
		if (idx_enter_node == -1)
		{
			enter_node = new NodeHeap(enter_id);
			heap_last_used.push_back(enter_id);

			enter_node->NodeHeap_Insert_Customer(cus);

			curr_size++;
			idx_last_node++;

			heap_array_of_node.push_back(enter_node);

			if (curr_size != 1)
				HEAP_Reheap_Up(idx_last_node);
		}
		// NODE that Customer enter IS IN THE HEAP ALREADY
		else
		{
			idx_enter_node = HEAP_Heap_Array_Find(enter_id); // find index of heap in heap_last_used vector

			enter_node = heap_array_of_node[idx_enter_node]; // get the heap node that the customer now enter

			idx_enter_node = HEAP_Heap_Last_Used_Find(enter_id);
			heap_last_used.erase(heap_last_used.begin() + idx_enter_node); // pop the heap node in the last_used vector
			heap_last_used.push_back(enter_id);							   // insert it at the top of the heap_last used vector
			enter_node->NodeHeap_Insert_Customer(cus);

			int idx = HEAP_Heap_Array_Find(enter_node->label_node); // find the index of the node that customer enter in the Heap Ary AND reheap down it as it increase key value
			HEAP_Reheap_Down(idx);
		}
	}
	// -----------------------------------------------------------------
	// KEITEIKEN FUNCTIONS
	// DELETE NUM CUSTOMER OF NUM AREAS
	bool HEAP_Compare_KEITEIKEN(NodeHeap *&n1, NodeHeap *&n2)
	{
		return n1->num_customer_in_node < n2->num_customer_in_node || (n1->num_customer_in_node == n2->num_customer_in_node && HEAP_Heap_Last_Used_Find(n1->label_node) < HEAP_Heap_Last_Used_Find(n2->label_node));
	}

	// NOT FINISH
	void HEAP_KEITEIKEN(int NUM)
	{
		int count_num = 0;
		int num_del_node = (NUM > int(heap_array_of_node.size())) ? (int(heap_array_of_node.size())) : (NUM);
		vector<NodeHeap *> vec_delete_nodes;
		for (int i = 0; i < (int)heap_array_of_node.size(); i++)
			vec_delete_nodes.push_back(heap_array_of_node[i]);

		if (vec_delete_nodes.size() == 0)
			return;

		// -----------------------------------------------------------------
		// PERFORM SORTING USING INSERTION SORT
		// -----------------------------------------------------------------
		// Sorting criteria: smallest number of customer in a node
		// If same number of cus, the one that is not used longer is the smaller one
		int i, j;
		NodeHeap *key;
		for (i = 1; i < (int)vec_delete_nodes.size(); i++)
		{
			key = vec_delete_nodes[i];
			j = i - 1;

			while (j >= 0 && HEAP_Compare_KEITEIKEN(key, vec_delete_nodes[j]))
			{
				vec_delete_nodes[j + 1] = vec_delete_nodes[j];
				j = j - 1;
			}
			vec_delete_nodes[j + 1] = key;
		}

		// -----------------------------------------------------------------
		// END INSERTION SORT
		// -----------------------------------------------------------------
		NodeHeap *del_node = vec_delete_nodes[0];

		while (count_num < num_del_node)
		{
			del_node = vec_delete_nodes[0];
			// Delete customer in the node WITHOUT affect propertiess of tree
			// As Number of Nodes in NodeHeap is greater than NUM, no need to REHEAP
			if (del_node->num_customer_in_node > NUM)
			{
				del_node->NodeHeap_Delete_NUM_Cus_FIFO(NUM);

				int idx_swap = HEAP_Heap_Last_Used_Find(del_node->label_node);
				heap_last_used.erase(heap_last_used.begin() + idx_swap);
				heap_last_used.push_back(del_node->label_node);
			}
			// Delete ALL CUSTOMERS of node => DELETE NODE
			else
			{
				del_node->NodeHeap_Delete_All_FIFO();

				NodeHeap *hold = heap_array_of_node[0];								  // hold the root
				heap_array_of_node[0] = heap_array_of_node[idx_last_node];			  // swap las_t node to the roto
				heap_array_of_node[idx_last_node] = hold;							  // put the root to the las_t node
				heap_array_of_node.erase(heap_array_of_node.begin() + idx_last_node); // pop that 'old root' at las_t

				idx_last_node--;	 // decrease las_t
				curr_size--;		 // decrease the size
				HEAP_Reheap_Down(0); // perform reheap down the new root to its right position
				// ----------------------------------------------------------------
				int idx_del_check = HEAP_Heap_Last_Used_Find(del_node->label_node);
				if (idx_del_check != -1)
				{
					heap_last_used.erase(heap_last_used.begin() + idx_del_check);
					delete del_node;
				}
				else
					cout << "Cannot find the NODE HEAP in heap_last_used" << endl;
			}

			// Erase the first node for deleted in the vec_del vector
			vec_delete_nodes.erase(vec_delete_nodes.begin());
			count_num++;
		}
	}

	// -----------------------------------------------------------------
	void HEAP_CLEAVE(int NUM)
	{
		help_CLEAVE(0, NUM);
	}
};
// -----------------------------------------------------------------
// HELP LAPSE METHODS
// -----------------------------------------------------------------
// Compare function - We compare the frequency AND the ascii property
// -----------------------------------------------------------------
bool LAPSE_Standardization(string name, map<char, char> &map_caesar, vector<pair<char, int>> &vec_for_build_after_merge)
{
	map<char, int> map_freq;

	for (int i = 0; i < int(name.length()); i++)
		map_freq[name[i]]++;

	// check if the name contains >= 3 different characters
	if (map_freq.size() < 3)
		return false;

	for (auto it : map_freq)
	{
		int temp_rotate = int(it.first);
		// ascii: a -> z == 97 -> 122
		if (temp_rotate >= 97)
		{
			temp_rotate = temp_rotate + (it.second % 26);
			if (temp_rotate <= 122)
				map_caesar[it.first] = char(temp_rotate);
			else
				map_caesar[it.first] = char(96 + (temp_rotate - 122));
		}
		// ascii: A -> Z == 65 -> 90
		else
		{
			temp_rotate = temp_rotate + (it.second % 26);
			if (temp_rotate <= 90)
				map_caesar[it.first] = char(temp_rotate);
			else
				map_caesar[it.first] = char(64 + (temp_rotate - 90));
		}
	}

	string test_caesar = "";
	for (int i = 0; i < int(name.length()); i++)
		test_caesar += map_caesar[name[i]];

	for (int i = 0; i < int(test_caesar.length()); i++)
	{
		if (i == 0)
			vec_for_build_after_merge.push_back({test_caesar[i], 1});
		else
		{
			bool found = 0;
			for (int j = 0; j < int(vec_for_build_after_merge.size()); j++)
			{
				if (vec_for_build_after_merge[j].first == test_caesar[i])
				{
					vec_for_build_after_merge[j].second += 1;
					found = 1;
				}
			}
			if (!found)
				vec_for_build_after_merge.push_back({test_caesar[i], 1});
		}
	}

	return true;
}

// COMMENT THIS FOR A MOMEMENT
HuffmanTree *LAPSE_Build_Tree_Calculate_Result(string name, int &int_result, map<char, char> &map_caesar, vector<pair<char, int>> &vec_for_build_after_merge, unordered_map<char, string> &huffmanCode)
{
	HuffmanTree *tree = new HuffmanTree();
	tree->HuffmanTree_Build_Tree(vec_for_build_after_merge);

	// traverse the Huffman Tree and store Huffman Codes in a map.
	tree->HuffmanTree_Encode(huffmanCode);

	// ADDED HERE FOR DECREASE TIME
	int name_check_len = int(name.length());
	name_check_len = (name_check_len > 10) ? 10 : name_check_len;
	name = name.substr(name.length() - name_check_len, name_check_len);

	string str = "";
	for (char ch : name)
		str += huffmanCode[map_caesar[ch]];

	int temp_length = int(str.length());
	temp_length = (temp_length < 10) ? (temp_length) : 10;

	string str_result = str.substr(str.length() - temp_length, temp_length);

	reverse(str_result.begin(), str_result.end());

	int_result = stoi(str_result, nullptr, 2);

	return tree;
}

class Restaurant_Control
{
public:
	class Restaurant_G
	{
	public:
		vector<BST_Tree *> area_of_restaurant_G;

		Restaurant_G()
		{
			for (int i = 0; i < MAXSIZE; i++)
				area_of_restaurant_G.push_back(new BST_Tree());
		}

		~Restaurant_G()
		{
			for (int i = 0; i < MAXSIZE; i++)
			{
				BST_Tree *temp = area_of_restaurant_G[i];
				delete temp;
				temp = nullptr;
			}
		}

		void Restaurant_G_Insert_Customer(Customer *enterCustomer)
		{
			int id_enter = enterCustomer->ID;
			area_of_restaurant_G[id_enter - 1]->BST_Tree_Insert(enterCustomer);
		}

		void Restaurant_G_KOKUSEN()
		{
			for (auto &area : area_of_restaurant_G)
				area->BST_Tree_KOKUSEN();
		}

		void Restaurant_G_LIMITLESS(int NUM)
		{
			if (NUM <= 0 || NUM > MAXSIZE)
				return;
			else
				area_of_restaurant_G[NUM - 1]->BST_Tree_Print_Inorder();
		}
	};

	class Restaurant_S
	{
	public:
		HEAP *area_of_restaurant_S;

		Restaurant_S()
		{
			this->area_of_restaurant_S = new HEAP();
		}

		~Restaurant_S()
		{
			delete area_of_restaurant_S;
			this->area_of_restaurant_S = nullptr;
		}

		void Restaurant_S_Insert_Customer(Customer *enterCustomer)
		{
			area_of_restaurant_S->HEAP_Insert_Customer(enterCustomer);
		}

		void Restaurant_S_KEITEIKEN(int NUM)
		{
			area_of_restaurant_S->HEAP_KEITEIKEN(NUM);
		}

		void Restaurant_S_CLEAVE(int NUM)
		{
			area_of_restaurant_S->HEAP_CLEAVE(NUM);
		}
	};

	Restaurant_S *res_s;
	Restaurant_G *res_g;

	Restaurant_Control()
	{
		this->res_s = new Restaurant_S();
		this->res_g = new Restaurant_G();
	}

	~Restaurant_Control()
	{
		delete res_s;
		this->res_s = nullptr;
		delete res_g;
		this->res_g = nullptr;
	}

	void LAPSE(string name)
	{
		// -----------------------------------------------------------------
		// CHECK IF CUSTOMER OK TO GO IN
		// -----------------------------------------------------------------
		map<char, char> map_caesar;
		vector<pair<char, int>> vec_for_build_after_merge; // { decoded_char, frequency }

		bool check_LAPSE = LAPSE_Standardization(name, map_caesar, vec_for_build_after_merge);

		if (!check_LAPSE)
		{
			return;
		}
		// -----------------------------------------------------------------
		// BUILD HUFFMAN TREE (NOT AVL PROPERTY YET) AND CALCULATE RESULT
		// -----------------------------------------------------------------
		unordered_map<char, string> huffmanCode;
		int int_result = 0;

		if (int(vec_for_build_after_merge.size()) == 1)
		{
			int_result = 0;
			int num_char = vec_for_build_after_merge[0].first;
			string pdz = "";
			pdz += char(num_char);
			global_name_for_hand = pdz + "\n";
		}
		else
		{
			HuffmanTree *tree = LAPSE_Build_Tree_Calculate_Result(name, int_result, map_caesar, vec_for_build_after_merge, huffmanCode);

			global_name_for_hand = tree->HuffmanTree_Print_HAND(tree->root);

			delete tree;
			tree = nullptr;
		}
		// -----------------------------------------------------------------
		// CHOOSE RESTAURANT and CHOOSE AREA IN THE RESTAURANT
		// -----------------------------------------------------------------
		Customer *enter_customer = new Customer(global_name_for_hand, int_result);

		if (enter_customer->result % 2 == 1)
			res_g->Restaurant_G_Insert_Customer(enter_customer);
		else
			res_s->Restaurant_S_Insert_Customer(enter_customer);
	}

	// -----------------------------------------------------------------
	// -----------------------------------------------------------------
	void KOKUSEN()
	{
		res_g->Restaurant_G_KOKUSEN();
	}

	// -----------------------------------------------------------------
	// -----------------------------------------------------------------
	void KEITEIKEN(int num)
	{
		res_s->Restaurant_S_KEITEIKEN(num);
	}

	// -----------------------------------------------------------------
	// -----------------------------------------------------------------
	void HAND()
	{
		if (global_name_for_hand == "")
			return;
		else
			printf("%s", global_name_for_hand.c_str());
	}

	// -----------------------------------------------------------------
	// -----------------------------------------------------------------
	void LIMITLESS(int NUM)
	{
		res_g->Restaurant_G_LIMITLESS(NUM);
	}

	// -----------------------------------------------------------------
	// -----------------------------------------------------------------
	void CLEAVE(int NUM)
	{
		res_s->Restaurant_S_CLEAVE(NUM);
	}
};
// -----------------------------------------------------------------
// -----------------------------------------------------------------

void simulate(string filename)
{
	ifstream ss(filename);
	string str, maxsize, name, num;
	Restaurant_Control *res_control = nullptr;
	while (ss >> str)
	{
		// ss >> str;
		if (str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
			res_control = new Restaurant_Control();
		}
		else if (str == "LAPSE")
		{
			ss >> name;
			res_control->LAPSE(name);
		}
		else if (str == "KOKUSEN")
		{
			res_control->KOKUSEN();
		}
		else if (str == "KEITEIKEN")
		{
			ss >> num;
			res_control->KEITEIKEN(stoi(num));
		}
		else if (str == "HAND")
		{
			res_control->HAND();
		}
		else if (str == "LIMITLESS")
		{
			ss >> num;
			res_control->LIMITLESS(stoi(num));
		}
		else if (str == "CLEAVE")
		{
			ss >> num;
			res_control->CLEAVE(stoi(num));
		}
	}
	global_name_for_hand = "";
	delete res_control;
}
// -----------------------------------------------------------------