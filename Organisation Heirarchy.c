#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"PythonList.c"

// The following is a employee in the organisation, it has the pointer to two more employees a subordinate_1 and a subordinate_2
struct Employee {
    int emp_id; // emp_ids will be unique
    char* emp_name;
    int emp_salary;
    struct Employee* subordinate_1;
    struct Employee* subordinate_2;
};

// The following function creates a employee and returns its pointer
struct Employee* create_employee(int id, char* name, int salary) {
    struct Employee* ptr = (struct Employee*) malloc(sizeof(struct Employee));
    ptr->emp_id = id;
    ptr->emp_salary = salary;
    ptr->emp_name = strdup(name);
    // strdup() creates a copy of the string or char pointer and stores it in the new char pointer of the employee
    ptr->subordinate_1 = NULL;
    ptr->subordinate_2 = NULL;
    return ptr;
}

// The following code creates a organisation from scanning the input file
struct Employee* create_company() {
    int id, salary;
    char name[100];
    scanf("%d", &id);
    if(id == -1) return NULL; // -1 is used when there is a NULL pointer ie when no value is present

    scanf("%s %d", name, &salary);
    struct Employee* par = create_employee(id, name, salary);

    par->subordinate_1 = create_company();
    par->subordinate_2 = create_company();
    
    return par;
}

// The following function 
void print_company_helper(struct Employee* ceo) {
    // take input
    if(!ceo) {
        printf("%d ", -1);
        return;
    }
    printf("%d %s %d ", ceo->emp_id, ceo->emp_name, ceo->emp_salary);
    print_company_helper(ceo->subordinate_1);
    print_company_helper(ceo->subordinate_2);
    return;
}

void print_company(struct Employee* ceo) {
    print_company_helper(ceo);
    printf("\n");
    return;
} 

struct Employee* CEO = NULL;
// CEO is a global pointer that points to the CEO of the company
struct Employee* search(struct Employee * finder ,int id){
    if (finder == NULL){
        return NULL;
    }
    else if (finder != NULL && finder->emp_id == id){
        return finder;
    }
    else{
        struct Employee* found = search(finder->subordinate_1,id);
        if (found != NULL){
            return found;
        }
        else{
            struct Employee* found_1 = search(finder->subordinate_2,id);
            if (found_1 != NULL){
                return found_1;
            }
        }
    }
}

struct Employee* helper_team_head(struct Employee* Team , int x){
    if (CEO->emp_id != x){
        if (Team == NULL){
            return NULL;
        }
        else if ((Team->subordinate_1 != NULL && Team->subordinate_1->emp_id == x ) || (Team->subordinate_2 != NULL && Team->subordinate_2->emp_id == x )){
            return Team;
        }
        else{
            struct Employee* boss_1 = helper_team_head(Team->subordinate_1,x);
            if (boss_1 != NULL){
                return boss_1;
            }
            else {
                struct Employee* boss_2 = helper_team_head(Team->subordinate_2,x);
                if (boss_2 != NULL){
                    return boss_2;
                }
            }
        }
    }
}

struct Employee* helper_boss(struct Employee* x , struct Employee* common , struct Employee* y){
    if(search(x,y->emp_id) != NULL){
        return x;
    }
    else{
        x = helper_team_head(CEO,x->emp_id);
        helper_boss(x,common,y);
    }
}

// The below function returns the employee id of the first common boss
int get_first_common_boss(int emp_id1,int emp_id2){
    struct Employee* search1 = search(CEO,emp_id1);
    struct Employee* search2 = search(CEO,emp_id2);
    struct Employee* first_num = search(search1,emp_id2);
    struct Employee* second_num = search(search2,emp_id1);
    if (CEO->emp_id != emp_id1 && CEO->emp_id != emp_id2){
        if (first_num != NULL){
            return emp_id1;
        }
        else if (second_num != NULL){
            return emp_id2;
        }
        else{
        struct Employee* common;
        common = helper_boss(search1,common,search2);
        return common->emp_id;
        }
    }
    else{
        return CEO->emp_id;
    }
}

// Print the bosses of the given employee in the order from CEO to immediate boss

void get_boss_id(struct Employee* immediate_boss , int x){
    if (immediate_boss == CEO){
        printf("%d ",immediate_boss->emp_id);
    }
    else{
        immediate_boss = helper_team_head(CEO,x);
        get_boss_id(immediate_boss,immediate_boss->emp_id);
        if (immediate_boss != CEO){
            printf("%d ",immediate_boss->emp_id);
        }
    }
}
void get_all_bosses(int emp_id){
    if (CEO->emp_id != emp_id){
    get_boss_id(helper_team_head(CEO,emp_id),emp_id);
    }
    else {
        printf("%d",-1);
    }
    return;
}

// Return the average salary of the team with the given employee as head
int salary = 0;
double salary_helper(struct Employee* person){
    if (person == NULL){
        salary = salary + 0;
        return salary;
    }
    else{
        salary = salary + person->emp_salary;
        salary_helper(person->subordinate_1);
        salary_helper(person->subordinate_2);
        return salary;
    }
}
int count_num = 0;

double person_counter(struct Employee* person){
    if (person == NULL){
        count_num = count_num + 0;
        return count_num;
    }
    else{
        count_num = count_num + 1 + person->emp_salary - person->emp_salary;
        person_counter(person->subordinate_1);
        person_counter(person->subordinate_2);
        return count_num;
    }
}

double get_average_salary(int emp_id){
    int salary_initial = salary ;
    struct Employee* employee = search(CEO,emp_id);
    double total_sum = salary_helper(employee) - salary_initial;
    int count_initial = count_num ;
    double total_employees = person_counter(employee) - count_initial;
    return (total_sum/total_employees);
}

int helper_level(struct Employee*head_lev ,int dist_ceo ,int x){
    if (CEO->emp_id != x){
        if (head_lev == NULL){
            return 0;
        }
        else if (head_lev->emp_id == x){
            return dist_ceo;
            
        }
        else if (head_lev->emp_id != x){
            int case1 =  helper_level(head_lev->subordinate_1,dist_ceo+1,x);
            if (case1 != 0){
                return case1;
            }
            else  {  
            int case2 =  helper_level(head_lev->subordinate_2,dist_ceo+1,x);
                if (case2 != 0){
                    return case2;
                }
            }
        }
    }
}
int Level(int emp_id) {
    if (CEO->emp_id != emp_id){

        int level = 0;
        return (helper_level(CEO,level,emp_id));
    }
    else{
        return 0;
    }
}
void name_helper(struct Employee* name , char x){
if (name == NULL){
       return;
    }
else if (name != NULL && *(name->emp_name) == x){
        append(name->emp_id);
    }
    name_helper(name->subordinate_1,x);
    name_helper(name->subordinate_2,x);
}

void sort_new() {
    int n = len();
    for(int i=0;i<n;i++) {
        for(int j=0;j<n-i-1;j++) {
            int x = Level(get_node_at_pos(j)->data);
            int y = Level(get_node_at_pos(j+1)->data);
            if(x > y) swap(j);
        }
    }
}

void same_last_names(int emp_id){
    char a = *(search(CEO,emp_id)->emp_name);
    clear();
    name_helper(CEO,a);
    sort_new();
    print_list(PythonListHead);
}

int main(int argc, char const *argv[])
{
    CEO = create_company();
    print_company(CEO);

    int T; 
    scanf("%d", &T);

    char operation_type[100];

    while(T--) {
        scanf("%s", operation_type);

        if(strcmp(operation_type, "get_first_common_boss") == 0) {
            int x,y;
            scanf("%d %d", &x, &y);
            int ans = get_first_common_boss(x,y);
            printf("%d\n", ans);
        } 
        else if(strcmp(operation_type, "same_last_names") == 0) {
            int x;
            scanf("%d", &x);
            same_last_names(x);
            printf("\n");
        } 
        else if(strcmp(operation_type, "get_all_bosses") == 0) {
            int x;
            scanf("%d", &x);
            get_all_bosses(x);
            printf("\n");
        } 
        else if(strcmp(operation_type, "get_average_salary") == 0) {
            int x;
            scanf("%d", &x);
            double ans = get_average_salary(x);
            printf("%.2f\n", ans);
        } 

    }

    return 0;
}
