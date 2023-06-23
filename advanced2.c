#include "monty.h"
/**
 * modop - multiplies the top two elements and stores it in second element
 * @stack: pointer to the head node pointer of stack
 * @nline: the line number
 * Return: Nothing.
 */
void modop(stack_t **stack, unsigned int nline)
{
	stack_t *temp;

	if (!(*stack) || !(*stack)->next)
	{
		fprintf(stderr, "L%d: can't mod, stack too short\n", nline);
		exit(EXIT_FAILURE);
	}

	if ((*stack)->n == 0)
	{
		fprintf(stderr, "L%d: division by zero\n", nline);
		exit(EXIT_FAILURE);
	}

	temp = *stack;
	(*stack)->next->n %= (*stack)->n;
	*stack = (*stack)->next;
	(*stack)->prev = NULL;
	free(temp);
}


/**
 * addst - changes mode of push to stack
 * @stack: pointer to stack
 * @nline: line number
 *
 * Return: Nothing.
 */
void addst(stack_t **stack, unsigned int nline)
{
	(void)nline;
	(void)stack;

	arg.flag = 0;
}

/**
 * addqu - changes mode of push to queue
 * @stack: pointer to stack
 * @nline: line number
 *
 * Return: Nothing.
 */
void addqu(stack_t **stack, unsigned int nline)
{
	(void)nline;
	(void)stack;

	arg.flag = 1;
}
