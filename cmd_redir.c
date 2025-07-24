/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:27:43 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/24 19:28:04 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *get_cmd_and_redir(t_token *token_list)
{
	int expand_h_doc = 0;
    t_token *final_token = NULL;
    t_token *tmp = token_list;
    t_token *pipe;
	// print_linked_list(token_list);

    while (tmp)
    {
        if (tmp->type != PIPE)
        {
            t_token *cmd_token = NULL;
            char **cmds = NULL;
            t_token *redir_head = NULL;
            t_token *redir_tail = NULL;
            int cmd_count = 0;
            int cmd_capacity = 8;

            cmds = malloc(sizeof(char *) * cmd_capacity);
            if (!cmds)
                return NULL;

            while (tmp && tmp->type != PIPE)
            {
                if (tmp->type == CMD || tmp->type == SINGLE_QUOTE || tmp->type == DOUBLE_QUOTE)
                {
                    if (cmd_count >= cmd_capacity)
                    {
                        cmd_capacity *= 2;
                        char **new_cmds = realloc(cmds, sizeof(char *) * cmd_capacity);
                        if (!new_cmds)
                        {
                            free(cmds);
                            return NULL;
                        }
                        cmds = new_cmds;
                    }
                    cmds[cmd_count++] = strdup(tmp->value);
                    tmp = tmp->next;
                }
                else if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT || tmp->type == APPEND || tmp->type == HEREDOC)
                {
					expand_h_doc = tmp->next->expand_heredoc;
                    t_token *redir_op = tmp;
                    t_token *redir_target = tmp->next;
                    if (!redir_target)
                        break;
                    t_token *redir_token = create_token(redir_target->value, 0, redir_target->has_space);
					redir_token->expand_heredoc = expand_h_doc;
                    redir_token->type = redir_op->type;
                    if (!redir_head)
                        redir_head = redir_token;
                    else
                        redir_tail->next = redir_token;
                    redir_tail = redir_token;
                    tmp = redir_target->next;
                }
                else
                    tmp = tmp->next;
            }
            cmds[cmd_count] = NULL;
            if (cmds && cmds[0])
            {
                cmd_token = create_token(cmds[0], 0, 0);
                cmd_token->type = CMD;
            }
            else
            {
                cmd_token = create_token("", 0, 0);
                cmd_token->type = CMD;
            }
            cmd_token->cmds = cmds;
            cmd_token->redir = redir_head;
            append_token(&final_token, cmd_token);
        }
        else if (tmp && tmp->type == PIPE)
        {
            pipe = create_token(tmp->value, 0, tmp->has_space);
            pipe->type = PIPE;
            append_token(&final_token, pipe);
            tmp = tmp->next;
        }
    }
    return final_token;
}
