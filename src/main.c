/** templ
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <err.h>

#define MODE_CONFIG (mode_t)( S_IRWXU | (S_IRGRP | S_IXGRP) | S_IXOTH )	/* 0751 */
#define MODE_DATA (mode_t)( S_IRWXU )	/* 0700 */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <regex.h>
#include <errno.h>
#include <assert.h>
#include <err.h>

int
mkdir_p( /* {{{1 */
    const char	*path,
    mode_t	 mode)
{
	char *dname, copy[1024];
	struct stat st;

	assert (path);

	if (stat(path, &st) != 0)	/* does not exist */
	    {	/* recurse through path */
		dname = dirname(strcpy(copy, path));	/* strip the basename */

		if (strcmp(dname, ".\0") != 0)	/* not at top directory */
		    mkdir_p(dname, mode);

		if (mkdir(path, mode) != 0)
		    errx(1,"mkdir failure: %s", strerror(errno));
	    }

	return 0;
}

int
main( /* {{{1 */
    int		argc,
    char *const	argv[])
{
	char *homedir, *confdir, *datadir;
	char buf[1024];
	struct stat st;
	FILE * tf; char *tf_name;
	FILE * df; char *df_name;
	regex_t reg_upper;
	regmatch_t pmatch[80]; int nmatch = 80;

	/* locate/initialize directories/files {{{2 */

	/* home directory */
	if (!(homedir = getenv("HOME")))
	    errx(1,"HOME environment variable is empty");

	/* configuration directory */
	if (!(confdir = getenv("XDG_CONFIG_HOME")))
	    {
		sprintf(buf, "%s/.config/templ", homedir);
	    }
	else
	    {
		sprintf(buf, "%s/templ", confdir);
	    }

	if (stat(buf, &st) != 0)
	    mkdir_p(buf, MODE_CONFIG);

	/* TODO: source templ config */

	/* data directory */
	if (!(datadir = getenv("XDG_DATA_HOME")))
	    {
		sprintf(buf, "%s/.local/share/templ", homedir);
	    }
	else
	    {
		sprintf(buf, "%s/templ", datadir);
	    }

	if (stat(buf, &st) != 0)
	    mkdir_p(buf, MODE_DATA);

	/* locate/initialize templ data subdirs */
	sprintf(buf, "%s/templ/templates", datadir);

	if (stat(buf, &st) != 0)
	    mkdir_p(buf, MODE_DATA);

	/* TODO: load data files (templates) when in interactive mode */

	/* copy template to file {{{2
	 * replacing xxx/Xxx/XXX with filename respecting case */

	/*
	tf_name = argv[1];
	df_name = argv[2];
	*/

	tf = fopen("test_template", "r");

	char fstr[1024];
	regcomp(&reg_upper, "XXX", 0);

	while (fgets(fstr, 1024, tf))
	    {
		char *s;

		s = fstr;

		printf("matching: %s\n", s);

		while (regexec(&reg_upper, s, nmatch, pmatch, 0) != REG_NOMATCH)
		    {
			char b[80];
			regoff_t so, eo, len;

			so = pmatch[0].rm_so;
			eo = pmatch[0].rm_eo;
			len = eo - so;

			s += so;

			snprintf(b, len+1, "%s", s);
			printf("matched: %s\toffsets: %d,%d\n", b, (int)so, (int)eo);

			s += len;
		    }
	    }

	regfree(&reg_upper);

	fclose(tf);





	return EXIT_SUCCESS;
} /* }}}1 */

/* vi: set ts=8 sw=8 noexpandtab: */
