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
#define BUFF_SIZE 100

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

int find_n(char *buf, int i) {
    int k;

    k = 0;
    while (k < i) {
        if (buf[k] == '\n')
            return (k);
        if (buf[k] == '\0')
            return (-2);
        ++k;
    }

    return (-1);
}

t_list *find_fd_list(t_list **lst, int fd) {
    t_list *temp_lst;
    t_list *temp;

    temp_lst = *lst;
    while (temp_lst != NULL)
    {
        if (temp_lst->content_size == fd)
            return (temp_lst);
        temp_lst = temp_lst->next;
    }
    if (!(temp = (t_list*) malloc(sizeof(t_list))))
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

int read_str(int fd, t_list *lst, char **line, char *buf) {
    int r;
    int pos;

    if (lst->content == NULL) {
        r = read(fd, buf, BUFF_SIZE);
            if( r == 0 || r == -1)
                return (!r ? 0 : -1);
        lst->content = ft_strsub(buf, 0, BUFF_SIZE);
    }
    while ((pos = find_n(lst->content, ft_strlen(lst->content) + 1)) == -1) {
        if (!(r = read(fd, buf, BUFF_SIZE)) == -1)
            return (-1);
        if (!(lst->content = ft_strjoin(lst->content, buf)))
            return (-1);
    }
  
    if (pos == -2) {
        *line = ft_strjoin("", lst->content);
        return (0);
    }
    *line = ft_strsub(lst->content, 0, pos);
    lst->content = ft_strsub(lst->content, pos + 1, ft_strlen(lst->content) - pos);
    return 1;
}

int get_next_line(const int fd, char **line) {
    static t_list *lst;
    t_list *t;
    char *buf;

    if (fd < 0 || BUFF_SIZE < 1)
        return (-1);
    if (!(t = find_fd_list(&lst, fd)))
        return (-1);
    if (!(buf = (char*) malloc(BUFF_SIZE + 1)))
        return (-1);
    buf[BUFF_SIZE] = 0;
    return (read_str(fd, t, line, buf));

}

int main(int argc, char** argv) {

    char *line;
    int i;
    int fd = open("C:/Users/Ivan/Desktop/t.txt", 0);

    int k = 0;
    while((i = get_next_line(fd, &line)))
    {
        printf("%d  %d  :::%s\n", ++k, i, line);
    }
    

    close(fd);
    return (EXIT_SUCCESS);
}

