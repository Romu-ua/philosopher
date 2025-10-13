#include <stdio.h>
#include <stdlib.h>

typedef struct s_node
{
	int				data;
	struct s_node	*next;
}	t_node;

typedef struct s_queue
{
	t_node	*front;
	t_node	*rear;
}	t_queue;

void	init(t_queue *q)
{
	q->front = NULL;
	q->rear = NULL;
}

int	is_empty(t_queue *q)
{
	if (!q->front)
		return (1);
	return (0);
}

void	enqueue(t_queue *q, int data)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	new_node->data = data;
	new_node->next = NULL;
	if (is_empty(q))
	{
		q->front = new_node;
		q->rear = new_node;
	}
	else
	{
		q->rear->next = new_node;
		q->rear = new_node;
	}
}

int	dequeue(t_queue *q)
{
	t_node	*tmp;
	int		value;

	if (is_empty(q))
	{
		printf("queue is empty\n");
		return (1);
	}
	tmp = q->front;
	value = tmp->data;
	q->front = tmp->next;
	if (q->front == NULL)
		q->rear = NULL;
	free(tmp);
	return (value);
	
}


int	main(void)
{
	t_queue	q;

	init(&q);

	enqueue(&q, 24);
	enqueue(&q, 42);
	printf("%d\n", dequeue(&q));
	printf("%d\n", dequeue(&q));
	return (0);
}