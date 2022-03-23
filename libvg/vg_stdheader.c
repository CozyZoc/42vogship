/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vg_stdheader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadue <amenadue@student.42adel.org.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 18:29:51 by amenadue          #+#    #+#             */
/*   Updated: 2022/03/24 00:37:07 by amenadue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vogship.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#ifndef STDHEADER_LENGTH
# define STDHEADER_LENGTH 80
#endif

#ifndef STDHEADER_MARGIN
# define STDHEADER_MARGIN 5
#endif

t_str ascii_art[7] = {
"        :::      ::::::::",
"      :+:      :+:    :+:",
"    +:+ +:+         +:+  ",
"  +#+  +:+       +#+     ",
"+#+#+#+#+#+   +#+        ",
"     #+#    #+#          ",
"    ###   ########.adl   "
};

t_str start = "/*";
t_str end = "*/";
char fill = '*';

t_str	stdhd_tformat()
{
	t_str timefmt;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	timefmt = (t_str) malloc(20 * sizeof(char));
	if (timefmt == NULL)
		return (NULL);
	sprintf(timefmt, "%04d/%02d/%02d %02d:%02d:%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	return ((t_str) timefmt);
}

t_str crepeat(char c, int n)
{
	t_str	tmp;
	int 	i;

	tmp = (t_str) malloc((n + 1) * sizeof(char));
	i = 0;
	while (i < n)
		tmp[i++] = c;
	return (tmp);
}

t_str text_line(t_str left, t_str right)
{
	t_str line;
	t_str buf;
	int marginleft;
	int marginmid;
	int marginright;
	int marginend;

	line = (t_str) malloc(STDHEADER_LENGTH+1 * sizeof(char));
	ft_strlcat(line, start, STDHEADER_MARGIN);
	marginend = STDHEADER_LENGTH - ft_strlen(end);
	marginright = STDHEADER_LENGTH - STDHEADER_MARGIN;
	marginmid = marginright - ft_strlen(right);
	marginleft = STDHEADER_MARGIN + ft_strlen(left);

	ft_strlcat(left, "!", ft_strlen(left) + 1);

	buf = crepeat(' ', STDHEADER_MARGIN);
	ft_strlcat(line, buf, STDHEADER_MARGIN+1);
	ft_strlcat(line, left, marginleft+1);
	buf = crepeat(' ', STDHEADER_LENGTH);
	ft_strlcat(line, buf, marginmid+1);
	ft_strlcat(line, right, marginright);
	buf = crepeat(' ', STDHEADER_MARGIN);
	ft_strlcat(line, buf, marginend+1);
	ft_strlcat(line, end, STDHEADER_LENGTH+1);
	ft_strlcat(line, "\n", STDHEADER_LENGTH+2);
	return (line);
}

t_str fill_line()
{
	t_str	line;
	t_str	tmp;
	int		fillwidth;

	line = (t_str) malloc((STDHEADER_LENGTH+1) * sizeof(char));
	ft_strlcat(line, start, STDHEADER_MARGIN);
	fillwidth = STDHEADER_LENGTH - ft_strlen(end) - ft_strlen(start) - 2;
	ft_strlcat(line, " ", STDHEADER_LENGTH+1);
	tmp = crepeat(fill, fillwidth);
	ft_strlcat(line, tmp, STDHEADER_LENGTH+1);
	ft_strlcat(line, " ", STDHEADER_LENGTH+1);
	ft_strlcat(line, end, STDHEADER_LENGTH+1);
	ft_strlcat(line, "\n", STDHEADER_LENGTH+2);

	return (line);
}

t_str	stdhd_user()
{
	t_str	tmp;

	tmp = vg_run("echo $USER");
	tmp = ft_strtrim(tmp, "\n");
	if (!tmp)
	{
		tmp = ft_strdup("marvin");
	}
	return (tmp);
}

t_str	stdhd_mail()
{
	t_str	tmp;

	tmp = vg_run("echo $MAIL");
	tmp = ft_strtrim(tmp, "\n");
	if (!ft_strncmp(tmp, "", 2))
		tmp = ft_strdup("marvin@42.fr");
	return (tmp);
}

int		vg_stdheader(t_str filepath)
{
	t_str header;
	t_str tmp;
	t_str filled;
	int c;

	FILE *fp;
	FILE *fpt;
	struct stat fst;

	stat(filepath, &fst);

	if (access( filepath, F_OK ) != 0)
	{
    	fprintf(stderr, "File doesn't exist!\n");
		return (0);
	}
	else if (S_ISDIR(fst.st_mode))
	{
		fprintf(stderr, "Cannot :Stdheader on a directory\n");
		return (0);
	}
	else
	{
		header = (t_str) malloc((STDHEADER_LENGTH * 12) * sizeof(char));
		if (header == NULL)
			return (0);

		fp = fopen(filepath, "r");
		fpt = fopen(filepath, "w");

		filled = fill_line();
		sprintf(header, "%s", filled);
		sprintf(header, "%s%s", header, text_line("", ""));
		sprintf(header, "%s%s", header, text_line("", ascii_art[0]));
		sprintf(header, "%s%s", header, text_line(filepath, ascii_art[1]));
		sprintf(header, "%s%s", header, text_line("", ascii_art[2]));
		tmp = (char *) calloc(STDHEADER_LENGTH, sizeof(char));
		if (tmp == NULL)
			return (0);
		sprintf(tmp, "By: %s <%s>", stdhd_user(), stdhd_mail());
		sprintf(header, "%s%s", header, text_line(tmp, ascii_art[3]));
		sprintf(header, "%s%s", header, text_line("", ascii_art[4]));
		sprintf(tmp, "Created: %s By %s", stdhd_tformat(), stdhd_user());
		sprintf(header, "%s%s", header, text_line(tmp, ascii_art[5]));
		sprintf(tmp, "Updated: %s By %s", stdhd_tformat(), stdhd_user());
		sprintf(header, "%s%s", header, text_line(tmp, ascii_art[6]));
		sprintf(header, "%s%s", header, text_line("", ""));
		sprintf(header, "%s%s\n", header, filled);
		while (0)
		{
			c = fgetc(fp);
			fputc(c, fpt);
			if (feof(fp))
				break;
		}
		tmp = (t_str) malloc(128 * sizeof(char));
		ft_strlcat(tmp, "cp /tmp/stdheader ", 19);
		ft_strlcat(tmp, filepath, 20 + ft_strlen(filepath));
		ft_strlcat(tmp, "; rm /tmp/stdheader", ft_strlen(filepath) + 40);
		system(tmp);
		return (1);
	}
}