/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsthead.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadue <amenadue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 23:47:59 by amenadue          #+#    #+#             */
/*   Updated: 2022/02/26 12:53:44 by amenadue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libvg/vogship.h"

const int	g_cmdindex = 5;

int hasfileext(t_str file, t_str set)
{
	int		i;
	int		j;
	int		setlen;
	t_str	buf;
	t_str	ext;

	i = 0;
	j = 0;
	setlen = ft_strlen(set);
	buf = (t_str) malloc(setlen * sizeof(char));
	while (i < setlen)
	{
		if (set[i] == ' ' || set[i] == '\0')
		{
			ext = ft_strdup(".");
			ft_strlcat(ext, buf, j+1);
			if (endswith(file, ext))
				return (1);
			ft_bzero(buf, setlen);
			j = 0;
		}
		else
		{
			buf[j] = set[i];
			j++;
		}
		i++;
	}
	return (0);
}

int	main(int c, t_str *v)
{
	t_str	line;
	int		flag;
	int		start_writing;
    size_t	len = 0;
    ssize_t	read;

    FILE    *fp;
	FILE	*nfp;
	int	i;
	
	i = 1;
	if (c == 1)
	{
		printf("Usage: ");
		vg_cmdhelp(g_cmdindex);
		nl();
	}
	else
	{
		while (i < c)
		{
			line = ft_strdup("ls ");
			ft_strlcat(line, v[i], 128);
			line = vg_run(line);
			if (endswith(line, "No such file or directory"))
			{
				printf("Couldn't find %s\n", v[i]);
			}
			else if (!strcmp(line, v[i]))
			{
				line = strdup("~/.vogship/bin/rsthead ");
				ft_strlcat(line, v[i], 128);
				ft_strlcat(line, "/*", 128);
				printf("%s: dir!\n", v[i]);
				vg_runp(line);
			}
			else 
			{
				if (hasfileext(v[i], "c cc h hh cpp hpp php")) {
					printf("%s: \n", v[i]);
					flag = 0;
					start_writing = 0;
					fp = fopen(v[i], "r");
					nfp = fopen("vgrmheader", "w");
					if (!fp) {
						fprintf(stderr, "Failed to open %s\n", v[i]);
						return (1);
					}
					if (!nfp) {
						fprintf(stderr, "Failed to open %s\n", v[i]);
						return (1);
					}

					line = (t_str) malloc(512 * sizeof(char));
					while (fgets(line, 512, fp) != NULL) {
						if (start_writing)
						{
							fprintf(nfp, "%s\n", line);
						}
						else 
						{
							if (flag >= 11)
							{
								if (!ft_strncmp(line, "", 1)) {
									start_writing = 1;
								}
							}
							if (startswith(line, "/*") && endswith(line, "*/"))
							{
								flag++;
							}
						}
					}
					fclose(fp);
					fclose(nfp);

					line = ft_strdup("mv vgrmheader ");
					ft_strlcat(line, v[i], 128);
					vg_run(line);
					vg_run("rm -rf vgrmheader");
					printf("%s: Removed header!\n", v[i]);
					if (line != NULL)
						free(line);
				} else {
					printf("%s: Invalid Format: skipped!\n", v[i]);
				}
			}
			i++;
		}

	}
	return (0);
}
