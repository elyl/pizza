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

void	load_file(t_pizza *pizza);
void	write_file();

int	read_number(int *dest, char *buffer);
int	veirfy_certificate(t_pizza *p, t_part *s);

char	*get_next_line(int fd);

#endif
