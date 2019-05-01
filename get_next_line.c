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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 1000

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

void ft_lstadd(t_list **alst, t_list *new) {
    if (!alst || !new)
        return;
    if (!*alst) {
        *alst = new;
    } else {
        new->next = *alst;
        *alst = new;
    }
}

t_list *ft_lstnew(void const *content, size_t content_size) {
    t_list *result;

    if (!(result = (t_list*) malloc(sizeof (t_list))))
        return (NULL);
    if (!content) {
        result->content = NULL;
        result->content_size = 0;
    } else {
        if (!(result->content = (char*) malloc(content_size))) {
            free(result);
            return (NULL);
        }
        ft_memcpy(result->content, content, content_size);
        result->content_size = content_size;
    }
    result->next = NULL;
    return (result);
}

void ft_clear(t_list *lst) {
    t_list *tmp;

    while (lst) {
        tmp = lst;
        lst = lst->next;
        free(tmp->content);
        tmp->content = NULL;
        free(tmp);
        tmp = NULL;
    }
}

size_t find_n(char *buf, size_t i) {
    size_t k;

    k = 0;
    while (k <= i) {
        if (buf[k] == '\n' || buf[k] == '\0')
        {
            //buf[k] = 0;
            return (k);
        }
        ++k;
    }
    return (-1);
}

t_list *find_fd_list(t_list *lst, int fd)
{
    t_list *temp_lst;
    t_list *temp;

    temp_lst = lst;
    while (temp_lst)
    {
        if (temp_lst->content_size == fd)
            return (temp_lst);
        temp_lst = temp_lst->next;
    }
    if (!(temp = (t_list*) malloc(sizeof (size_t))))
        return (NULL);
    if (!(temp->content = (char*) malloc(1)))
        return (NULL);
    ((char*) (temp->content))[0] = 0;
    temp->content_size = fd;
    temp->next = NULL;
    if (!lst)
        return (lst = temp);
    temp_lst = lst;
    while(temp_lst->next)
        temp_lst = temp_lst->next;
    temp_lst->next = temp;
    return (temp);
}



int get_next_line(const int fd, char **line) {
    static t_list *lst;
    t_list *t;
    char *buf;
    char *str;
    int r;
    size_t pos;

    if (fd < 0 || BUFF_SIZE < 1)
        return (-1);
    if(!(t = find_fd_list(lst, fd)))
        return (-1);
    if (!(buf = (char*)malloc(BUFF_SIZE + 1)))
        return (-1);
    buf[BUFF_SIZE] = 0;
    while ((r = read(fd, buf, BUFF_SIZE))) {
        if (r <= 0)
            return (r == 0 ? 0 : -1);
        if ((pos = find_n(buf, r)) != -1)
        {
            if (!(str = ft_strsub(buf, 0, pos)))
                return (-1);
            if(!(*line = ft_strjoin(t->content, str)))
                return (-1);
            free(str);
            free(t->content);
            t->content = ft_strsub(buf, pos, BUFF_SIZE - pos);
            return (1);
        }else
        {
            str = t->content;
            if(!(t->content = ft_strjoin(t->content, buf)))
                return (-1);
            free(str);
        }
    }
}

int main(int argc, char** argv) {
    
    char *line;
    
    int fd = open("C:/Users/Ivan/Desktop/t.txt", 0);
    
    while(get_next_line(fd, &line), line);
    printf("5  %d     :%s\n", line);
    close(fd);
    return (EXIT_SUCCESS);
}

