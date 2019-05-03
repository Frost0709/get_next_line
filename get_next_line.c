/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   get_next_line.c
 * Author: Ivan
 *
 * Created on 28 апреля 2019 г., 20:04
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 12

typedef struct s_list {
    void *content;
    int content_size;
    struct s_list *next;
} t_list;

void *ft_memcpy(void *dst, const void *src, size_t n) {
    while (n--)
        ((char*) dst)[n] = ((char*) src)[n];
    return (dst);
}

size_t ft_strlen(const char *s) {
    size_t l;

    l = 0;
    while (s[l])
        ++l;
    return (l);
}

char *ft_strjoin(char const *s1, char const *s2) {
    char *result;
    size_t s1_len;
    size_t s2_len;
    size_t i;

    if (!s1 || !s2)
        return (NULL);
    s1_len = ft_strlen(s1);
    s2_len = ft_strlen(s2);
    i = 0;
    if (!(result = (char*) malloc(s1_len + s2_len + 1)))
        return (NULL);
    while (*s1)
        result[i++] = *s1++;
    while (*s2)
        result[i++] = *s2++;
    result[i] = 0;
    return (result);
}

char *ft_strsub(char const *s, unsigned int start, size_t len) {
    char *result;
    size_t i;

    i = 0;
    if (!s)
        return (NULL);
    if (!(result = (char*) malloc(len + 1)))
        return (NULL);
    while (i < len) {
        result[i] = s[i + start];
        i++;
    }
    result[i] = 0;
    return (result);
}

void clear_all(t_list **lst, char **b) {
    t_list *tmp;
    
    free(*b);
    *b == NULL;
    while (*lst) {
        tmp = *lst;
        *lst = (*lst)->next;
        close(tmp->content_size);
        free(tmp->content);
        tmp->content = NULL;
        free(tmp);
        tmp = NULL;
    }
}

long find_n(char *buf, int i) {
    long k;

    k = 0;
    while (k < i) {
        if (buf[k] == '\n')
            return (k);
        ++k;
    }
    return (-1);
}

t_list *find_fd_list(t_list **lst, int fd) {
    t_list *temp_lst;
    t_list *temp;

    temp_lst = *lst;
    while (temp_lst != NULL) {
        if (temp_lst->content_size == fd)
            return (temp_lst);
        temp_lst = temp_lst->next;
    }
    if (!(temp = (t_list*) malloc(sizeof (t_list))))
        return (NULL);
    temp->content = NULL;
    temp->content_size = fd;
    temp->next = NULL;
    if (!*lst)
        return (*lst = temp);
    temp_lst = *lst;
    while (temp_lst->next)
        temp_lst = temp_lst->next;
    temp_lst->next = temp;
    return (temp);
}

int read_str(int fd, t_list *l, char **line, char *b, int r) {
    long p; 
    char *t;

    p = -2;
    while (r && (p = find_n(l->content, ft_strlen(l->content))) == -1) {   
            if ((r = read(fd, b, BUFF_SIZE)) == -1)
                return (-1);
            if (!(t = ft_strsub(b, 0 , r)))
                return (-1);
            if (!(l->content = ft_strjoin(l->content, t)))
            {
                free(t);
                return (-1);
            }
    }
    if (p == -1) {
        *line = l->content;
        l->content = NULL;
    } else {
        if (!(*line = ft_strsub(l->content, 0, p)))
            return (-1);
        if (!(l->content = ft_strsub(l->content, p + 1, ft_strlen(l->content) - p - 1)))
            return (-1);
    }
    return (1);
}

int get_next_line(const int fd, char **line) {
    static t_list *lst;
    t_list *l;
    char *b;
    int r;

    r = 1;
    if (fd < 0 || BUFF_SIZE < 1 || !(l = find_fd_list(&lst, fd)))
        return (-1);
    if (!(b = (char*) malloc(BUFF_SIZE + 1)))
        return (-1);
    b[BUFF_SIZE] = 0; 
    if (!l->content) {
        if ((r = read(fd, b, BUFF_SIZE)) <= 0)
            return (!r ? 0 : -1);
        if (!(l->content = ft_strsub(b, 0, r)))
            return (-1);
    }  
    if ((r = read_str(fd, l, line, b, r)) == -1)
        clear_all(&lst, &b);
    free(b);
    b = NULL;
    return (r);
}

int main(int argc, char** argv) {

    char *line;
    int i;
    int fd = open("C:/Users/Ivan/Desktop/t.txt", 0);

    int k = 0;
    while ((i = get_next_line(fd, &line))) {
        printf("%d  :::%s\n", i, line);
    }

    return (EXIT_SUCCESS);
}

