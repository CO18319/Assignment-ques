/*Post transition*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 6

struct package
{
	char* id;
	int weight;
};

typedef struct package package;

struct post_office
{
	int min_weight;
	int max_weight;
	package* packages;
	int packages_count;
};

typedef struct post_office post_office;

struct town
{
	char* name;
	post_office* offices;
	int offices_count;
};

typedef struct town town;

void print_all_packages(town t) 
{
    int i,j;
    printf("%s:\n",t.name);
    for(i=0; i<t.offices_count; i++)
    {
        printf("\t%d:\n",i);
        for(j=0; j<t.offices[i].packages_count; j++)
            printf("\t\t%s\n",t.offices[i].packages[j].id);
    }
}

void send_all_acceptable_packages(town* source, int source_office_index, town* target, int target_office_index) 
{	
    int max = target->offices[target_office_index].max_weight;
    int min = target->offices[target_office_index].min_weight;
    package* temp = NULL;
    int notaccept = 0, weight;
    post_office* s = source->offices + source_office_index;
    post_office* d = target->offices + target_office_index;
                         
    for(int i=0; i < s->packages_count; i++)
    {
        weight = s->packages[i].weight;
        if(weight <= max && weight >= min)
        {
            d->packages_count++;
            d->packages = (package*)realloc(d->packages, sizeof(package) * d->packages_count);
            d->packages[d->packages_count-1] = s->packages[i];         
        }
        else
        {
            notaccept++;
            temp = (package*)realloc(temp, sizeof(package) * notaccept);
            temp[notaccept-1] = s->packages[i];
        }
    }
    s->packages = temp;
    s->packages_count = notaccept;
}

int number_of_packages(town t)
{
    int ans = 0;
    for (int i = 0; i < t.offices_count; i++)
        ans += t.offices[i].packages_count;
    return ans;
}

town town_with_most_packages(town* towns, int towns_count) 
{
    int i,sol;
    int maxpackage=-1;
    for(i=0;i<towns_count;i++)
       if(number_of_packages(towns[i])>maxpackage)
       {
           maxpackage=number_of_packages(towns[i]);
           sol=i;
       }
    return towns[sol];
}

town* find_town(town* towns, int towns_count, char* name) 
{
    int i;
    for(i=0;i<towns_count;i++)
    {
        if(!strcmp(towns[i].name,name))
            return &(towns[i]); 
    }       
           
    return &towns[0];
    }
    

int main()
{
	int towns_count;
	scanf("%d", &towns_count);
	town* towns = malloc(sizeof(town)*towns_count);
	for (int i = 0; i < towns_count; i++) {
		towns[i].name = malloc(sizeof(char) * MAX_STRING_LENGTH);
		scanf("%s", towns[i].name);
		scanf("%d", &towns[i].offices_count);
		towns[i].offices = malloc(sizeof(post_office)*towns[i].offices_count);
		for (int j = 0; j < towns[i].offices_count; j++) {
			scanf("%d%d%d", &towns[i].offices[j].packages_count, &towns[i].offices[j].min_weight, &towns[i].offices[j].max_weight);
			towns[i].offices[j].packages = malloc(sizeof(package)*towns[i].offices[j].packages_count);
			for (int k = 0; k < towns[i].offices[j].packages_count; k++) {
				towns[i].offices[j].packages[k].id = malloc(sizeof(char) * MAX_STRING_LENGTH);
				scanf("%s", towns[i].offices[j].packages[k].id);
				scanf("%d", &towns[i].offices[j].packages[k].weight);
			}
		}
	}
	int queries;
	scanf("%d", &queries);
	char town_name[MAX_STRING_LENGTH];
	while (queries--) {
		int type;
		scanf("%d", &type);
		switch (type) {
		case 1:
			scanf("%s", town_name);
			town* t = find_town(towns, towns_count, town_name);
			print_all_packages(*t);
			break;
		case 2:
			scanf("%s", town_name);
			town* source = find_town(towns, towns_count, town_name);
			int source_index;
			scanf("%d", &source_index);
			scanf("%s", town_name);
			town* target = find_town(towns, towns_count, town_name);
			int target_index;
			scanf("%d", &target_index);
			send_all_acceptable_packages(source, source_index, target, target_index);
			break;
		case 3:
			printf("Town with the most number of packages is %s\n", town_with_most_packages(towns, towns_count).name);
			break;
		}
	}
	return 0;
}
