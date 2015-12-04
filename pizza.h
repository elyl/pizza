#ifndef __PIZZA_H__
#define __PIZZA_H__

#define BUFFER_SIZE	255
#define MAX_NUMBER	5

typedef struct s_pizza
{
  int	r;
  int	c;
  int	h;
  int	s;
  char	**f;
}	t_pizza;

typedef struct s_part
{
  int	r1;
  int	r2;
  int	c1;
  int	c2;
}	t_part;

typedef struct s_list
{
  t_part *s;
  struct s_list *next;
}	t_list;

t_list	*add_to_list(t_list *list, t_part *s);
t_list	*generate_slices(t_pizza *p);
t_list	*find_solution(t_list *list);

void	load_file(t_pizza *pizza);
void	write_file();
void	free_list(t_list *list);
void	display_list(t_list *list);
void	free_tab(t_pizza *p);
void	print_list(t_list *list);

int	read_number(int *dest, char *buffer);
int	veirfy_certificate(t_pizza *p, t_part *s);
int	is_compatible(t_list *list, t_part *new);

char	*get_next_line(int fd);

#endif
