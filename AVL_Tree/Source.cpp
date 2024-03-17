#include "iostream"
using namespace std;

struct nodeAVL {
	int data;
	nodeAVL* left;
	nodeAVL* right;
	int height;
};

nodeAVL* makeNode(int value) {
	nodeAVL* pointer = new nodeAVL();
	pointer->data = value;
	pointer->left = NULL;
	pointer->right = NULL;
	pointer->height = 1;
	return pointer;
}

int height(nodeAVL* root) {
	if (root == NULL) return 0;
	return root->height;
}

nodeAVL* search(nodeAVL* root, int value) {
	if (root == NULL || root->data == value)
		return root;
	else if (value < root->data)
		return search(root->left, value);
	else
		return search(root->right, value);
}

//------------------------------------------------------------------------------------------
//							XỬ LÝ XOAY 
// Bị lệch qua trái
void rotateRight(nodeAVL*& root) {
	if (root == NULL || root->left == NULL) return;
	nodeAVL* pointer = root->left;
	root->left = pointer->right;
	pointer->right = root;

	// update height of root
	int leftRoot = height(root->left);
	int rightRoot = height(root->right);
	root->height = max(leftRoot, rightRoot) + 1;

	// update height of pointer
	int leftPointer = height(pointer->left);
	int rightPointer = height(pointer->right);
	pointer->height = max(leftPointer, rightPointer) + 1;

	root = pointer;
}

// Bị lệch qua phải
void rotateLeft(nodeAVL*& root) {
	if (root == NULL || root->right == NULL) return;
	nodeAVL* pointer = root->right;
	root->right = pointer->left;
	pointer->left = root;

	// update height of root
	int leftRoot = height(root->left);
	int righRoot = height(root->right);
	root->height = max(leftRoot, righRoot) + 1;

	// update height of pointer
	int leftPointer = height(pointer->left);
	int rightPointer = height(pointer->right);
	pointer->height = max(leftPointer, rightPointer) + 1;

	root = pointer;
}

// Trong cây con trái bị lệch qua phải
void rotateLeftRight(nodeAVL*& root) {
	if (root == NULL || root->left == NULL) return;
	rotateLeft(root->left);
	rotateRight(root);
}

// Trong cây con phải bị lệch qua trái 
void rotateRightLeft(nodeAVL*& root) {
	if (root == NULL || root->right == NULL) return;
	rotateRight(root->right);
	rotateLeft(root);
}

// Cân bằng qua phải cho cây bị lệch trái
void balanceRight(nodeAVL*& root) {
	if (root == NULL) return;
	// Nếu độ cao lệch 2 đơn vị (không thể lệch 3 do mỗi lần chèn đều thực hiện cân bằng)
	if (height(root->left) - height(root->right) == 2) {
		nodeAVL* pointer = root->left;
		if (height(pointer->left) >= height(pointer->right))
			rotateRight(root);
		else
			rotateLeftRight(root);
	}
}

// Cân bằng qua trái cho cây bị lệch phải
void balanceLeft(nodeAVL*& root) {
	if (root == NULL) return;
	if (height(root->right) - height(root->left) == 2) {
		nodeAVL* pointer = root->right;
		if (height(pointer->right) >= height(root->left))
			rotateLeft(root);
		else
			rotateRightLeft(root);
	}
}

//------------------------------------------------------------------------------------------

void insert(nodeAVL*& root, int value) {
	if (root == NULL)
		root = makeNode(value);
	else {
		if (value < root->data) {
			insert(root->left, value);
			balanceRight(root);
		}
		else {
			insert(root->right, value);
			balanceLeft(root);
		}

		// update height of root
		int leftRoot = height(root->left);
		int rightRoot = height(root->right);
		root->height = max(leftRoot, rightRoot) + 1;
	}
}

void deleteNode(nodeAVL*& root, int key) {
	if (root == NULL) return;
	if (key == root->data) {
		if (root->left == NULL) {
			nodeAVL* temp;
			temp = root;
			root = root->right;
			delete temp;
		}
		else if (root->right == NULL) {
			nodeAVL* temp;
			temp = root;
			root = root->left;
			delete temp;
		}
		else {
			nodeAVL* minRightNode = root->left;
			while (minRightNode->right != NULL)
				minRightNode = minRightNode->right;
			root->data = minRightNode->data;
			deleteNode(root->left, minRightNode->data);
			balanceLeft(root);
		}
	}
	else if (key < root->data) {
		deleteNode(root->left, key);
		balanceLeft(root); // Gọi hàm cân bằng lại cây khi xóa nút bên trái
	}
	else {
		deleteNode(root->right, key);
		balanceRight(root); // Gọi hàm cân bằng lại cây khi xóa nút bên phải
	}

	// Cập nhật độ cao của nút 
	// Kiểm tra khác NULL là do có TH nút xóa là nút lá khi xóa nó là NULL khi cập nhật sẽ lỗi 
	if (root != NULL)
		root->height = 1 + max(height(root->left), height(root->right));
}

void NLR(nodeAVL* root) {
	if (root) {
		cout << root->data << " ";
		NLR(root->left);
		NLR(root->right);
	}
}

int main() {
	nodeAVL* root = NULL;
	const int number = 11;
	int value[number] = { 10,12,1,14,6,5,8,15,3,9,7 };
	for (int i = 0; i < number; i++) {
		insert(root, value[i]);
	}
	cout << "\nLNR: ";
	NLR(root);

	deleteNode(root, 9);
	cout << "\nLNR after deletion: ";
	NLR(root);

	return 0;
}