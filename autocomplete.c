#include "autocomplete.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int compare_terms(const void *p_a, const void *p_b)
{
    term *p_a_s = (term *)p_a;
    term *p_b_s = (term *)p_b;
    return strcmp(p_a_s->term, p_b_s->term);

}

int compare_weight(const void *p_a, const void *p_b)
{
    term *p_a_s = (term *)p_a;
    term *p_b_s = (term *)p_b;
    return p_b_s->weight - p_a_s->weight;
}

void read_in_terms(term **terms, int *pnterms, char *filename)
{
    // retrieve the number of terms from the first line in the document 
    FILE *fp = fopen(filename, "r");
    char line[200];
    fgets(line, 200, fp);
    line[strlen(line) - 1] = '\0';
    *pnterms = atoi(line);

    // allocate the space for the terms in memory
    *terms = (term *)malloc(*pnterms * sizeof(term));
    
    int i = 0;
    while(fgets(line, 200, fp) != NULL){
        char term_word[200];
        char term_number[200];
        int j = 0;
        int k = 0;
        while(line[j] != '\t'){
            term_number[j] = line[j];
            j++;
        }
        term_number[j] = '\0';
        while(line[j + k + 1] != '\n'){
            term_word[k] = line[j + k + 1];
            k++;
        }
        term_word[k] = '\0';

        // put the word into struct
        strcpy((*(*terms + i)).term, term_word);
        // put the weight into struct
        (*(*terms + i)).weight = atof(term_number);  
        

        i++;
    }
    fclose(fp);

    // sort the terms
    qsort(*terms, *pnterms, sizeof(term), compare_terms); 
    
}

int lowest_match(term *terms, int nterms, char *substr)
{
    int left = 0;
    int right = nterms - 1;
    while(left <= right){
        int mid = (left + right - (left + right) % 2) / 2;    // to get an integer, not a float

        int i = 0;
        char cmp_word[200];
        while(*(substr + i) != '\0'){
            cmp_word[i] = ((*(terms + mid)).term)[i];   
            i++;
        }
        cmp_word[i] = '\0';    

        if(strcmp(substr, cmp_word) > 0){
            left = mid + 1;
        } else{
            right = mid - 1;
        }
    }

    return left;
}

int highest_match(struct term *terms, int nterms, char *substr)
{
    int left = 0;
    int right = nterms - 1;
    while(left <= right){
        int mid = (left + right - (left + right) % 2) / 2;    // to get an integer, not a float

        int i = 0;
        char cmp_word[200];
        while(*(substr + i) != '\0'){
            cmp_word[i] = ((*(terms + mid)).term)[i];  
            i++;
        }
        cmp_word[i] = '\0';

        if(strcmp(substr, cmp_word) < 0){
            right = mid - 1;
        } else{
            left = mid + 1;
        }
    }

    return right;
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
    int low = lowest_match(terms, nterms, substr);
    int high = highest_match(terms, nterms, substr);

    *n_answer = (high - low) + 1;

    *answer = (term *)malloc(*n_answer * sizeof(term));

    int i = 0;
    while(low <= high){
        strcpy((*(*answer + i)).term, (*(terms + low)).term);
        (*(*answer + i)).weight = (*(terms + low)).weight;
        i++;
        low++;
    }

    qsort(*answer, *n_answer, sizeof(term), compare_weight);

}



int main(void)
{
    term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "cities1.txt");

    // why are the weights different
    int i = 0;
    while(i < nterms){
        printf("%s \t", (*(terms + i)).term);
        printf("%f \n", (*(terms + i)).weight);
        i++;
    }



    int low_term = lowest_match(terms, nterms, "Mu");
    int high_term = highest_match(terms, nterms, "Mu");
    printf("low term: %d \n", low_term);
    printf("high term: %d \n", high_term);
    

    term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "Mu");
    //free allocated blocks here -- not required for the project, but good practice

    printf("After sorting \n");
    int j = 0;
    while(j < n_answer){
        printf("%s \t", (*(answer + j)).term);
        printf("%f \n", (*(answer + j)).weight);
        j++;
    }

    free(terms);
    free(answer);

    return 0;
}



