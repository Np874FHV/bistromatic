#include "declarations.h"

static size_t minimum(size_t a, size_t b)
{
    if (a < b)
        return a;
    return b;
}

void pop_token_list(struct token_list **tl)
{
    if (!tl || !*tl)
        return;
    struct token_list *new_head = (*tl)->next;
    destroy_number((*tl)->number);
    free(*tl);
    *tl = new_head;
}

struct number *init_number(size_t whole_part_size, size_t decimal_part_size)
{
    struct number *res = malloc(sizeof(struct number));
    if (!res)
        return NULL;
    res->whole_part_size = whole_part_size;
    res->decimal_part_size = decimal_part_size;
    res->whole_part = calloc(whole_part_size, sizeof(int));
    if (!res->whole_part)
        goto error_init_number;
    if (decimal_part_size > 0)
    {
        res->decimal_part = calloc(decimal_part_size, sizeof(int));
        if (!res->decimal_part)
            goto error_init_number;
    }
    else
        res->decimal_part = NULL;
    res->positive = 1;
    return res;

error_init_number:
    if (res->whole_part)
        free(res->whole_part);
    free(res);
    return NULL;
}

struct temporary_number *init_temporary_number(size_t whole_part_capacity,
                                               size_t decimal_part_capacity)
{
    struct temporary_number *res = malloc(sizeof(struct temporary_number));
    if (!res)
        return NULL;
    res->whole_part_capacity = whole_part_capacity;
    res->whole_part_size = 0;
    res->decimal_part_capacity = decimal_part_capacity;
    res->decimal_part_size = 0;
    res->whole_part = malloc(whole_part_capacity * sizeof(int));
    if (!res->whole_part)
        goto error_init_temporary_number;
    res->decimal_part = malloc(decimal_part_capacity * sizeof(int));
    if (decimal_part_capacity > 0 && !res->decimal_part)
        goto error_init_temporary_number;
    return res;

error_init_temporary_number:
    if (res->whole_part)
        free(res->whole_part);
    if (res->decimal_part_capacity > 0 && res->decimal_part)
        free(res->decimal_part);
    destroy_temporary_number(res);
    return NULL;
}

int resize_temporary_number_whole(struct temporary_number *tempnum)
{
    tempnum->whole_part_capacity = 2 * tempnum->whole_part_capacity;
    int *tmp = realloc(tempnum->whole_part,
                       tempnum->whole_part_capacity * sizeof(int));
    if (!tmp)
        return 1;
    tempnum->whole_part = tmp;
    return 0;
}

int resize_temporary_number_decimal(struct temporary_number *tempnum)

{
    tempnum->decimal_part_capacity = 2 * tempnum->decimal_part_capacity;
    int *tmp = realloc(tempnum->decimal_part,
                       tempnum->decimal_part_capacity * sizeof(int));
    if (!tmp)
        return 1;
    tempnum->decimal_part = tmp;
    return 0;
}

void destroy_token_list(struct token_list *tl)
{
    if (!tl)
        return;
    if (tl->number)
        destroy_number(tl->number);
    destroy_token_list(tl->next);
    free(tl);
}

void destroy_temporary_number(struct temporary_number *tempnum)
{
    if (!tempnum)
        return;
    if (tempnum->whole_part)
        free(tempnum->whole_part);
    if (tempnum->decimal_part)
        free(tempnum->decimal_part);
    free(tempnum);
}

void destroy_number(struct number *num)
{
    if (!num)
        return;
    if (num->whole_part)
        free(num->whole_part);
    if (num->decimal_part)
        free(num->decimal_part);
    free(num);
}

static int *reverse_list(int *l, size_t size)
{
    int tmp;
    for (size_t i = 0; i < size / 2; i++)
    {
        tmp = l[i];
        l[i] = l[size - i - 1];
        l[size - i - 1] = tmp;
    }
    return l;
}

struct number *convert_from_temporary(struct temporary_number *tempnum)
{
    if (!tempnum)
        return NULL;
    struct number *num = malloc(sizeof(struct number));
    if (!num)
        return NULL;
    num->whole_part =
        reverse_list(tempnum->whole_part, tempnum->whole_part_size);
    num->whole_part_size = tempnum->whole_part_size;
    num->decimal_part = tempnum->decimal_part;
    num->decimal_part_size = tempnum->decimal_part_size;
    num->positive = 1;
    normalize(num);
    free(tempnum);
    return num;
}

void normalize(struct number *num)
{
    size_t i = num->whole_part_size - 1;
    while (i > 0 && num->whole_part[i] == 0)
    {
        i--;
    }
    num->whole_part_size = i + 1;
    if (num->decimal_part_size > 0)
    {
        i = num->decimal_part_size - 1;
        while (i > 0 && num->decimal_part[i] == 0)
        {
            i--;
        }
        if (i == 0 && num->decimal_part[i] == 0)
            num->decimal_part_size = 0;
        else
            num->decimal_part_size = i + 1;
        num->decimal_part =
            realloc(num->decimal_part, num->decimal_part_size * sizeof(int));
        if (num->decimal_part_size == 0)
            num->decimal_part = NULL;
    }
    num->whole_part =
        realloc(num->whole_part, num->whole_part_size * sizeof(int));
    if (num->whole_part_size == 1 && num->decimal_part_size == 0
        && num->whole_part[0] == 0) // num == 0
        num->positive = 1;
}

void print_number(struct number *num, const char *base, FILE *output)
{
    // printf("WHOLE: %ld ; DECIMAL : %ld\n", num->whole_part_size,
    // num->decimal_part_size);
    if (!num->positive)
        fprintf(output, "-");
    for (size_t i = num->whole_part_size - 1; i > 0; i--)
        fprintf(output, "%c", base[num->whole_part[i]]);
    fprintf(output, "%c", base[num->whole_part[0]]);
    if (num->decimal_part_size > 0)
        fprintf(output, ".");
    for (size_t i = 0; i < num->decimal_part_size; i++)
        fprintf(output, "%c", base[num->decimal_part[i]]);
}

static void print_oper(enum oper p)
{
    switch (p)
    {
    case PLUS:
        printf("+\n");
        break;
    case MINUS:
        printf("-\n");
        break;
    case POS:
        printf("POS\n");
        break;
    case NEG:
        printf("NEG\n");
        break;
    case MULT:
        printf("*\n");
        break;
    case DIV:
        printf("/\n");
        break;
    case EUCLDIV:
        printf("//\n");
        break;
    case MOD:
        printf("%c\n", '%');
        break;
    case POW:
        printf("^\n");
        break;
    case OPEN:
        printf("(\n");
        break;
    case CLOSE:
        printf(")\n");
        break;

    default:
        printf("OTHER OPER\n");
        break;
    }
}

void print_token_list(struct token_list *list, const char *base)
{
    if (!list)
        return;
    if (list->number)
        print_number(list->number, base, stdout);
    else
        print_oper(list->oper);
    print_token_list(list->next, base);
}

static struct token_list *reverse_token_list_aux(struct token_list *tl)
{
    if (!tl->next)
        return tl;
    struct token_list *new_head = reverse_token_list_aux(tl->next);
    tl->next->next = tl;
    return new_head;
}

struct token_list *reverse_token_list(struct token_list *tl)
{
    if (!tl)
        return NULL;
    struct token_list *new_head = reverse_token_list_aux(tl);
    tl->next = NULL;
    return new_head;
}

void transfer(struct token_list **sender, struct token_list **receiver)
{
    if (!sender || !receiver)
    {
        fprintf(stderr, "Error\n");
        return;
    }
    if (!*sender)
        return;
    struct token_list *tmp = *sender;
    *sender = tmp->next;
    tmp->next = *receiver;
    *receiver = tmp;
}

void swap_size_t(size_t *a, size_t *b)
{
    size_t c = *a;
    *a = *b;
    *b = c;
}

void swap_int(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

void swap_int_ptr(int **a, int **b)
{
    int *c = *a;
    *a = *b;
    *b = c;
}

void swap_numbers_in_place(struct number *a, struct number *b)
{
    swap_int_ptr(&(a->whole_part), &(b->whole_part));
    swap_size_t(&(a->whole_part_size), &(b->whole_part_size));
    swap_int_ptr(&(a->decimal_part), &(b->decimal_part));
    swap_size_t(&(a->decimal_part_size), &(b->decimal_part_size));
    swap_int(&(a->positive), &(b->positive));
}

int absolute_greater(struct number *a,
                     struct number *b) //>0 = True, <0 = False, 0 = equal
{
    if (a->whole_part_size > b->whole_part_size)
        return 1;
    if (a->whole_part_size < b->whole_part_size)
        return -1;
    size_t i = a->whole_part_size - 1;
    while (i > 0)
    {
        if (a->whole_part[i] != b->whole_part[i])
            return a->whole_part[i] - b->whole_part[i];
        i--;
    }
    if (a->whole_part[0] != b->whole_part[0])
        return a->whole_part[0] - b->whole_part[0];
    while (i < minimum(a->decimal_part_size, b->decimal_part_size))
    {
        if (a->decimal_part[i] != b->decimal_part[i])
            return a->decimal_part[i] - b->decimal_part[i];
        i++;
    }
    if (a->decimal_part_size == b->decimal_part_size)
        return 0;
    if (a->decimal_part_size > b->decimal_part_size)
        return 1;
    return -1;
}

int is_whole(struct number *num)
{
    return num->decimal_part_size == 0;
}
