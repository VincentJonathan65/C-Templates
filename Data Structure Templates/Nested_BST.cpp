#include<stdio.h>
#include<stdlib.h>

struct NodeTree{
    int value;
    struct NodeTree *left_child, *right_child;
};

struct NodeNestedTree{
    int id;
    struct NodeTree *root;
    struct NodeNestedTree *left_child, *right_child;
} *root_nested_tree;

void get_enter(){
    printf(" Enter to Continue..."); getchar();
}

int menu(){
    int choice;
    for(;;){ //endless loop
        system("CLS || clear");
        puts("1. Insert Value");
        puts("2. Delete Value");
        puts("3. Delete Root");
        puts("4. Print");
        puts("5. Update");
        puts("6. Clear All Root");
        puts("0. Exit");
        printf(">> "); scanf("%d", &choice); getchar();

        if(choice >= 0 && choice <= 6){
            return choice;
        }
        else{
            printf("Menu doesn't exist!"); get_enter();
        }
    }
}

///////////////////////////////////////////////////////////// BAGIAN TREE /////////////////////////////////////////////////////////////////

struct NodeTree *create_node(int value){
    struct NodeTree *new_node = (struct NodeTree *) malloc(sizeof(struct NodeTree));
    new_node->value = value;
    new_node->left_child = new_node->right_child = NULL;
    return new_node;
}

//masukkan node ke dalam suatu tree
struct NodeTree *insert_node(struct NodeTree *new_node, struct NodeTree *current_node){
    if(current_node == NULL){ //tempat yang pas untuk insert
        current_node = new_node;
        return current_node;
    }
    else{ //ada isinya
        if(new_node->value < current_node->value){
            current_node->left_child = insert_node(new_node, current_node->left_child);
        }
        else if(new_node->value > current_node->value){
            current_node->right_child = insert_node(new_node, current_node->right_child);
        }
        else{
            puts("Data already exist!");
            get_enter();
        }
        return current_node;
    }
}

//print value suatu tree
void pre_order(struct NodeTree *current_node){
    if(current_node == NULL){
        return;
    }
    printf("%d ", current_node->value);
    pre_order(current_node->left_child);
    pre_order(current_node->right_child);
}

void in_order(struct NodeTree *current_node){
    if(current_node == NULL){
        return;
    }
    in_order(current_node->left_child);
    printf("%d ", current_node->value);
    in_order(current_node->right_child);
}

void post_order(struct NodeTree *current_node){
    if(current_node == NULL){
        return;
    }
    post_order(current_node->left_child);
    post_order(current_node->right_child);
    printf("%d ", current_node->value);
}

void print_tree(struct NodeTree *root){
    printf("Pre-Order: "); pre_order(root); puts("");
    printf("In-Order: "); in_order(root); puts("");
    printf("Post-Order: "); post_order(root); puts("");
}

//predecessor & successor dari node di suatu tree
struct NodeTree *predecessor_tree(struct NodeTree *target_delete){
    struct NodeTree *predecessor_node = target_delete->left_child;
    while(predecessor_node->right_child != NULL){
        predecessor_node = predecessor_node->right_child;
    }
    return predecessor_node;
}

struct NodeTree *successor_tree(struct NodeTree *target_delete){
    struct NodeTree *successor_node = target_delete->right_child;
    while(successor_node->left_child != NULL){
        successor_node = successor_node->left_child;
    }
    return successor_node;
}

//delete suatu node di suatu tree
struct NodeTree *delete_node(struct NodeTree *current_node, int target_value){
    if(current_node == NULL){
        puts("Value to delete not exist!");
        get_enter();
        return current_node;
    }
    if(target_value < current_node->value){
        current_node->left_child = delete_node(current_node->left_child, target_value);
    }
    else if(target_value > current_node->value){
        current_node->right_child = delete_node(current_node->right_child, target_value);
    }
    else if(target_value == current_node->value){ //ketemu value-nya
        if(current_node->left_child == NULL && current_node->right_child == NULL){
            free(current_node);
            return NULL;
        }
        else if(current_node->left_child != NULL){
            // ========== Teknik Predecessor ============
            struct NodeTree *penerus = predecessor_tree(current_node);

            //swap
            int temp = penerus->value;
            penerus->value = current_node->value;
            current_node->value = temp;

            current_node->left_child = delete_node(current_node->left_child, target_value);
        }
        else if(current_node->right_child != NULL){
            //========== Teknik Successor ============
            struct NodeTree *penerus = successor_tree(current_node);

            //swap
            int temp = penerus->value;
            penerus->value = current_node->value;
            current_node->value = temp;

            current_node->right_child = delete_node(current_node->right_child, target_value);
        }
    }
    return current_node;
}

//hapus suatu tree dari nested tree
struct NodeTree *clear_tree(struct NodeTree *root){
    while(root != NULL){
        root = delete_node(root, root->value);
    }
    return root;
}

///////////////////////////////////////////////////////// BAGIAN NESTED TREE /////////////////////////////////////////////////////////////////

//bagaikan hash function di hash table
int get_root_identity(int value){
    return value / 10;
}

struct NodeNestedTree *create_root(struct NodeTree *new_node){
    struct NodeNestedTree *new_root = (struct NodeNestedTree *) malloc(sizeof(struct NodeNestedTree));
    new_root->root = new_node;
    new_root->id = get_root_identity(new_node->value);
    new_root->left_child = new_root->right_child = NULL;
    return new_root;
}

//masukkan data ke dalam suatu tree di nested_tree
struct NodeNestedTree *insert_nested_tree(struct NodeTree *new_node, struct NodeNestedTree *current_node){
    if(current_node == NULL){ //tempat yang pas untuk insert
        current_node = create_root(new_node);
        return current_node;
    }
    else{ //ada isinya
        if(get_root_identity(new_node->value) < get_root_identity(current_node->root->value)){
            current_node->left_child = insert_nested_tree(new_node, current_node->left_child);
        }
        else if(get_root_identity(new_node->value) > get_root_identity(current_node->root->value)){
            current_node->right_child = insert_nested_tree(new_node, current_node->right_child);
        }
        else{ //root ditemukan
            current_node->root = insert_node(new_node, current_node->root);
        }
        return current_node;
    }
}

//print semua tree di nested tree
void print_nested_tree(struct NodeNestedTree *current_node){ //pre-order
    if(current_node == NULL){
        return;
    }

    printf("===== Root %d =====\n", current_node->id);
    print_tree(current_node->root); puts("");

    print_nested_tree(current_node->left_child);
    print_nested_tree(current_node->right_child);
}

//predecessor & successor dari suatu root di nested tree
struct NodeNestedTree *predecessor_nested_tree(struct NodeNestedTree *target_delete){
    struct NodeNestedTree *predecessor_node = target_delete->left_child;
    while(predecessor_node->right_child != NULL){
        predecessor_node = predecessor_node->right_child;
    }
    return predecessor_node;
}

struct NodeNestedTree *successor_nested_tree(struct NodeNestedTree *target_delete){
    struct NodeNestedTree *successor_node = target_delete->right_child;
    while(successor_node->left_child != NULL){
        successor_node = successor_node->left_child;
    }
    return successor_node;
}

//hapus suatu root yang ada di nested tree
struct NodeNestedTree *delete_root(struct NodeNestedTree *current_node, int root_id){
    if(current_node == NULL){
        get_enter();
        return current_node;
    }
    if(root_id < current_node->id){
        current_node->left_child = delete_root(current_node->left_child, root_id);
    }
    else if(root_id > current_node->id){
        current_node->right_child = delete_root(current_node->right_child, root_id);
    }
    else{ //ketemu rootnya
        // current_node = execute_delete_root(current_node);
        current_node->root = clear_tree(current_node->root);
        if(current_node->left_child == NULL && current_node->right_child == NULL){
            free(current_node);
            return NULL;
        }
        else if(current_node->left_child != NULL){
            //========== Teknik Predecessor ============
            struct NodeNestedTree *penerus = predecessor_nested_tree(current_node);

            struct NodeTree *temp_root = current_node->root;
            current_node->root = penerus->root;
            penerus->root = temp_root;

            int temp_id = current_node->id;
            current_node->id = penerus->id;
            penerus->id = temp_id;

            current_node->left_child = delete_root(current_node->left_child, root_id);
        }
        else if(current_node->right_child != NULL){
            //========== Teknik Successor ============
            struct NodeNestedTree *penerus = successor_nested_tree(current_node);

            struct NodeTree *temp_root = current_node->root;
            current_node->root = penerus->root;
            penerus->root = temp_root;

            int temp_id = current_node->id;
            current_node->id = penerus->id;
            penerus->id = temp_id;

            current_node->right_child = delete_root(current_node->right_child, root_id);
        }
    }
    return current_node;
}

//hapus data dari suatu tree yang ada di nested tree
struct NodeNestedTree *delete_value(struct NodeNestedTree *current_node, int target_value){
    if(current_node == NULL){
        puts("Value to delete not exist!");
        get_enter();
        return current_node;
    }
    if(get_root_identity(target_value) < current_node->id){
        current_node->left_child = delete_value(current_node->left_child, target_value);
    }
    else if(get_root_identity(target_value) > current_node->id){
        current_node->right_child = delete_value(current_node->right_child, target_value);
    }
    else if(get_root_identity(target_value) == current_node->id){ //ketemu rootnya
        current_node->root = delete_node(current_node->root, target_value);

        if(current_node->root == NULL){
            current_node = delete_root(current_node, current_node->id);
        }
    }
    return current_node;
}

//hapus semua data dan root yang ada di nested tree
struct NodeNestedTree *clear_nested_tree(struct NodeNestedTree *root_nested_tree){
    while(root_nested_tree != NULL){
        // root_nested_tree = delete_value(root_nested_tree, root_nested_tree->root->value);
        root_nested_tree = delete_root(root_nested_tree, root_nested_tree->id);
    }
    return root_nested_tree;
}

int main(){
    while(1 == 1){
        int choice = menu();
        switch(choice){
            case 1:{ //insert
                int target;
                printf("Isi Value: "); scanf("%d", &target); getchar();
                struct NodeTree *newNode = create_node(target);
                root_nested_tree = insert_nested_tree(newNode, root_nested_tree);
                break;
            }
            case 2:{ //delete value
                int target;
                printf("Target Delete: "); scanf("%d", &target); getchar();
                root_nested_tree = delete_value(root_nested_tree, target);
                break;
            }
            case 3:{ //delete root
                int target;
                printf("Target Delete: "); scanf("%d", &target); getchar();
                root_nested_tree = delete_root(root_nested_tree, get_root_identity(target));
                break;
            }
            case 4:{ //print
                print_nested_tree(root_nested_tree);
                get_enter();
                break;
            }
            case 5:{ //update
                int target_lama;
                printf("Target Update: "); scanf("%d", &target_lama); getchar();
                root_nested_tree = delete_value(root_nested_tree, target_lama);
                
                int target_baru;
                printf("New Value: "); scanf("%d", &target_baru); getchar();
                struct NodeTree *new_node = create_node(target_baru);
                root_nested_tree = insert_nested_tree(new_node, root_nested_tree);
                break;
            }
            case 6:{ //clear all
                root_nested_tree = clear_nested_tree(root_nested_tree);
                break;
            }
            case 0:{ //exit
                printf("Program Terminated Well!");
                return 0;
            }
        }
    }
}

/*
1
20
1
30
1
10
1
23
1
33
1
13
*/