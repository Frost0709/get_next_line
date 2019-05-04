/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarre <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 22:11:47 by mbarre            #+#    #+#             */
/*   Updated: 2019/05/04 23:12:01 by mbarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

long	find_n(char *buf, int i)
{
	long	k;

	k = 0;
	while (k < i)
	{
		if (buf[k] == '\n')
			return (k);
		++k;
	}
	return (-1);
}

t_list	*find_fd_list(t_list **lst, int fd)
{
	t_list	*temp_lst;
	t_list	*temp;

	temp_lst = *lst;
	while (temp_lst != NULL)
	{
		if ((int)temp_lst->content_size == fd)
			return (temp_lst);
		temp_lst = temp_lst->next;
	}
	if (!(temp = (t_list*)malloc(sizeof(t_list))))
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

int		result_str(long p, t_list *l, char **line)
{
	char *t;

	if (p == -1)
	{
		*line = l->content;
		l->content = NULL;
		return (1);
	}
	if (!(*line = ft_strsub(l->content, 0, p)))
	{		
		free(l->content);
		l-content = NULL;
		return (-1);
	}
	t = l->content;
	l->content = ft_strsub(l->content, p + 1, ft_strlen(l->content) - p - 1);
	free(t);
	if (!l->content)
		return (-1);
	return (1);
}

int		read_str(int fd, t_list *l, char **line, char *b)
{
	int		r;
	long	p;
	char	*t;
	char	*t2;

	p = -2;
	r = -2;
	while (r && (p = find_n(l->content, ft_strlen(l->content))) == -1)
	{
		if ((r = read(fd, b, BUFF_SIZE)) == -1)
			return (-1);
		if (!(t = ft_strsub(b, 0, r)))
			return (-1);
		t2 = l->content;
		l->content = ft_strjoin((l->content), t);
		free(t);
		free(t2);
		if (!l->content)
			return (-1);
	}
	return (result_str(p, l, line));
}

int		get_next_line(const int fd, char **line)
{
	static t_list	*lst;
	t_list			*l;
	char			*b;
	int				r;

	r = 1;
	if (fd < 0 || BUFF_SIZE < 1 || !(l = find_fd_list(&lst, fd)))
		return (-1);
	if (!(b = (char*)malloc(BUFF_SIZE + 1)))
		return (-1);
	b[BUFF_SIZE] = 0;
	if (!l->content || *((char*)l->content) == 0)
	{
		if ((r = read(fd, b, BUFF_SIZE)) <= 0)
			return (!r ? 0 : -1);
		if (!(l->content = ft_strsub(b, 0, r)))
			return (-1);
	}
	r = read_str(fd, l, line, b);
	free(b);
	b = NULL;
	return (r);
}
