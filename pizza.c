#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "pizza.h"

char *get_next_line(int fd)
{
  static char	big_buffer[BUFFER_SIZE * 4 + 1] = {0};
  char		buffer[BUFFER_SIZE + 1];
  char		*out;
  int		n;
  int		i;

  n = read(fd, buffer, BUFFER_SIZE);
  i = 0;
  while (big_buffer[i] != 0)
    ++i;
  strncpy(&big_buffer[i], buffer, n);
  big_buffer[i + n] = '\0';
  i = 0;
  while (big_buffer[i] != '\0' && big_buffer[i] != '\n')
    ++i;
  if (big_buffer[i] != '\n' && n == BUFFER_SIZE)
    return (get_next_line(fd));
  out = malloc(i + 1);
  strncpy(out, big_buffer, i);
  out[i] = '\0';
  strcpy(big_buffer, &big_buffer[i + 1]);
  return (out);
}

void load_file(t_pizza *p)
{
  int	fd;
  int	n;
  int	i;
  char	*tmp;

  fd = open("input_file", O_RDONLY);
  tmp = get_next_line(fd);
  n = 0;
  n = read_number(&p->r, tmp) + 1;
  n += read_number(&p->c, &tmp[n]) + 1;
  n += read_number(&p->h, &tmp[n]) + 1;
  n += read_number(&p->s, &tmp[n]) + 1;
  free(tmp);
  p->f = malloc(p->r * sizeof(char*));
  i = 0;
  while (i < p->r)
    {
      p->f[i] = malloc(p->c);
      ++i;
    }
  i = 0;
  while (i < p->r)
    {
      tmp = get_next_line(fd);
      memcpy(p->f[i], tmp, p->c);
      free(tmp);
      ++i;
    }
}

int read_number(int *dest, char *buffer)
{
  char	tmp[MAX_NUMBER + 1];
  int	i;

  i = 0;
  while (isdigit(buffer[i]))
    ++i;
  strncpy(tmp, buffer, i);
  tmp[i] = '\0';
  *dest = atoi(tmp);
  return (i);
}

int verify_certificate(t_pizza *p, t_part *s)
{
  int	i;
  int	j;
  int	n;
  
  if ((s->r1 - s->r2) * (s->c1 - s->c2) > p->s)
    return (1);
  if (s->r1 >= p->r || s->r2 >= p->r || s->c1 >= p->c || s->c2 >= p->c)
    return (3);
  i = s->r2;
  n = 0;
  while (i <= s->r1)
    {
      j = s->c2;
      while (j <= s->c1)
	{
	  if (p->f[i][j] == 'H')
	    ++n;
	  ++j;
	}
      ++i;
    }
  if (n < p->h)
    return (2);
  return (0);
}

t_list *add_to_list(t_list *list, t_part *s)
{
  while (list->next != NULL)
    list = list->next;
  list->next = malloc(sizeof(t_list));
  list = list->next;
  list->s = s;
  list->next = NULL;
  return (list);
}

t_list *generate_slices(t_pizza *p)
{
  t_list	*first;
  t_list	*prev;
  t_part	*s;
  int		i;
  int		j;
  int		i2;
  int		j2;
  int		n;

  first = malloc(sizeof(t_list));
  first->next = NULL;
  i = 0;
  n = 0;
  while (i < p->r)
    {
      j = 0;
      while (j < p->c)
	{
	  i2 = 0;
	  while (i2 < p->s)
	    {
	      j2 = 0;
	      while (j2 < p->s)
		{	  
		  s = malloc(sizeof(t_part));
		  s->r1 = i + i2;
		  s->r2 = i;
		  s->c1 = j + j2;
		  s->c2 = j;
		  if (verify_certificate(p, s) != 0)
		    free(s);
		  else
		    {
		      add_to_list(first, s);
		      ++n;
		    }
		  ++j2;
		}
	      ++i2;
	    }
	  ++j;
	}
      ++i;
    }
  prev = first;
  first = first->next;
  free(prev);
  return (first);
}

int is_compatible(t_list *list, t_part *new)
{
  while (list != NULL)
    {
      if ((new->r2 >= list->s->r2 && new->r2 <= list->s->r1) || (new->r1 >= list->s->r2 && new->r1 <= list->s->r1))
	if ((new->c2 >= list->s->c2 && new->c2 <= list->s->c1) || (new->c1 >= list->s->c2 && new->c1 <= list->s->c1))
	  return (0);
      list = list->next;
    }
  return (1);
}

void display_list(t_list *list)
{
  while (list != NULL)
    {
      printf("{r1: %d; r2: %d; c1: %d;c2: %d}\n", list->s->r1, list->s->r2, list->s->c1, list->s->c2);
      list = list->next;
    }
}

void free_list(t_list *list)
{
  t_list	*prev;
  
  while (list != NULL)
    {
      prev = list;
      list = list->next;
      free(prev->s);
      free(prev);
    }
}

void free_list2(t_list *list)
{
  t_list	*prev;

  while (list != NULL)
    {
      prev = list;
      list = list->next;
      free(prev);
    }
}

void free_tab(t_pizza *p)
{
  int	i;

  i = 0;
  while (i < p->c)
    free(p->f[i++]);
  free(p->f);
}

void print_list(t_list *list)
{
  while (list != NULL)
    {
      list = list->next;
    }
}

t_list *find_solution(t_list *list)
{
  t_list	*first;
  t_list	*prev;

  first = malloc(sizeof(t_list));
  first->next = NULL;
  while (list != NULL)
    {
      if(is_compatible((first->next == NULL) ? NULL : first->next, list->s) == 1)
	add_to_list(first, list->s);
      list = list->next;
    }
  prev = first;
  first = first->next;
  free(prev);
  return (first);
}

int main()
{
  t_pizza	p;
  t_list	*list;
  t_list	*solution;

  load_file(&p);
  list = generate_slices(&p);
  solution = find_solution(list);
  display_list(solution);
  free_list(list);
  free_list2(solution);
  free_tab(&p);
  return (0);
}
