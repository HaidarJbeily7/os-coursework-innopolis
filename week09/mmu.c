#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define PAGETABLE "/tmp/ex2/pagetable"
#define PAGE_SIZE 8

int hit_count = 0;
int miss_count = 0;
int waiter = 1;

char str[] = "W337 R490 R194 R973 R151 R425 W722 R816 W331 W760 W864 R53 R416 W170 W829 W434 W181 R731 R86 R544 R29 R559 R629 R411 W365 W368 W802 W962 R529 R833 W8 R435 R564 W94 R64 W695 W654 R913 R867 R679 W129 R773 W158 R231 W477 R118 W825 W976 R616 R395 R715 R870 W229 R687 R20 R160 R543 W231 R977 W792 R942 W680 W81 R257 R314 R374 W354 W522 W464 R410 R462 W311 W82 W648 R299 R604 W588 R527 R131 W48 R32 W682 R295 W832 W131 W35 W246 R148 R256 R970 W510 W668 R305 W356 W265 R5 R581 R436 W411 R864 R59 R444 R603 R392 R778 R42 W91 R740 W229 W774 R6 R253 R729 R182 R649 R666 R313 R198 W65 W540 W392 W420 R132 W623 W155 R119 W781 R263 R752 R385 R809 W906 R790 R270 W960 R983 R881 R320 W122 W764 W861 R806 R555 R665 R191 R160 R192 R304 W129 R586 R207 W635 R300 R776 W281 W301 R724 W619 R64 R627 W934 W943 R380 W521 R910 W67 W902 W863 R500 R140 W227 R33 W51 W707 W523 W596 W572 R816 W715 W314 R843 W861 R694 W996 R850 W945 W592 W699 R912 R284 R361 W518 R529 R850 R4 W118 R248 W976 R827 W412 R586 R817 W305 R561 R731 R877 R450 R988 W563 R901 R23 R554 R873 W277 R703 R290 W53 W613 W713 W77 W45 R850 R68 W202 W717 W70 W865 W194 W2 W420 R240 R819 R581 W193 R353 R138 R890 W579 W138 R483 W131 R9 W237 R427 W60 W82 W163 R842 R538 W213 W648 R759 R837 R122 W262 R657 W334 R569 R668 W381 R695 R553 W388 R814 R952 W292 R290 R243 R399 R495 W32 R178 R770 W436 W26 W467 W431 W456 R762 R446 R462 W277 W398 R557 W98 R434 R893 W606 W91 R972 W176 W923 W85 R592 W960 R420 W662 W354 W990 R859 W699 R96 W814 W640 R263 W776 W437 W886 W447 R879 W236 R571 W693 W289 R708 W121 R792 R228 W273 W620 W408 W644 R190 W973 W348 W708 W461 R748 R835 R530 W790 W63 W26 W219 R881 R540 R947 R142 R337 R882 W521 W647 W833 R437 R643 R6 R995 W499 R530 R688 R83 R491 W675 R190 W259 W858 R975 R281 R814 W396 W435 R354 W54 W165 R110 R219 W861 W40 W15 R866 R10 W418 W520 R858 R903 R526 R113 W811 W62 R140 W84 W466 W316 W144 W54 R926 W714 R814 R470 W315 R615 R177 W200 W691 R704 R901 R301 R569 R568 W249 W767 W124 R161 R56 W695 W263 W924 R283 W575 R539 W368 R383 R436 R872 R809 W631 R267 R119 R146 W551 R178 W909 R450 R742 W992 W940 R314 W439 R252 W499 R540 W868 W156 W249 W605 R156 R986 R422 R981 W146 R882 W246 R651 R865 R746 W661 W767 W41 R948 W298 R356 W857 W865 W637 W520 R480 W215 W553 R155 W414 R33 W849 R797 W214 R248 R189 R362 R903 W965 W602 R615 W652 W222 R955 W858 W239 R889 W427 R331 R225 W477 R247 W646 W705 W10 W136 W38 R304 W587 R75 R159 W717 R702 W653 R509 W15 R167 R628 W918 W900 W322 W492 W587 R260 W241 R65 W647 W882 W855 R906 R755 W335 R881 W490 R700 R774 R249 R285 R714 W12 R688 R509 W665 R352 W482 W776 W799 R477 W931 R540 W104 W726 W28 R140 W759 W594 W19 W368 R52 R995 W480 W946 W53 R725 W115 W362 W726 W65 W925 W805 W348 W295 R206 W959 W144 R151 W869 R551 W996 W517 R22 R580 W853 W471 R424 W569 W311 R936 R12 R72 R83 R971 W354 W226 R717 W986 W294 R110 W835 R52 R877 W161 W435 W887 W474 W350 W7 R364 R962 R991 W445 R498 W156 R59 W967 W832 W796 W751 R753 R270 R994 W294 W123 W941 R422 W214 W920 W245 W672 W816 R652 W120 W460 R86 R610 W640 R848 W992 W70 R419 W575 W798 W837 R358 R856 R270 W883 R975 W944 W279 W767 W222 R156 W356 R772 R475 R880 W883 W733 W176 W449 R310 R8 R804 W465 W470 R276 W528 W809 W8 R250 W160 R691 R124 R621 W74 W766 R626 W938 W316 R499 R668 R417 R603 W435 W515 W4 W483 R121 R408 R204 W359 R941 W893 R130 W480 R920 W281 R875 R319 R633 R920 R7 R935 R244 R388 W235 R872 R224 R377 W12 R480 R493 W570 W969 W395 R460 R738 R997 W516 R386 W772 R559 W935 R401 W83 W711 R794 R321 R407 W511 W53 W862 R889 R797 R472 R584 W18 W695 R995 W636 R710 R77 R388 W296 R157 W934 R584 W285 W451 R920 R111 R780 W787 W166 R868 W903 R247 R860 W378 W319 W490 W943 W556 R164 R254 R378 R982 W85 R107 R195 W504 W903 R516 R911 R575 W509 W836 W31 W189 W119 W549 W911 R343 W661 W330 W752 R196 W483 R571 W619 R353 W285 R746 R454 W936 R288 R895 W271 W82 R863 W832 R547 R597 R731 R506 R706 W85 W873 W220 W532 W158 W879 R682 R380 R778 R299 R729 R162 W730 W511 W265 R935 W34 R293 W285 R923 R113 W563 R253 W367 W159 W848 R484 R633 W127 R124 R164 W303 W177 R478 W806 W99 W26 W779 W989 W268 W103 W751 W496 W424 R880 R596 R11 R797 W53 R104 W948 W917 W231 W576 R968 W867 W950 R858 W713 W878 W119 R148 R227 W58 W400 R1 R297 R655 W215 W691 W435 R601 R978 R333 R654 R293 W491 W672 W836 W553 R403 W371 W21 R397 W954 R31 W460 W106 W989 W328 R931 R610 R185 W570 R32 R773 W746 R658 W607 R316 R61 R444 R844 R157 R814 W648 R781 R623 W246 W489 W432 W232 W201 R542 W863 R344 W439 W646 W537 W621 R525 R51 R773 W401 W380 R546 W185 W136 R505 W321 R238 W956 R567 R565 W605 R457 R854 W169 W113 R956 R941 R643 W537 R628 R187 R62 R472 R709 W192 R442 R419 W264 R923 W694 W309 W13 W82 W178 R429 W411 R144 R994 R531 R85 W883 W922 W551 R647 R351 W824 R269 W508 W417 W138 R586 R923 W182 W170 W563 R457 R641 R862 R424 R473 W589 W694 W142 R772 W983 W972 W761 W266 W471 R985 W97 R67 W861 R758 R584 R214 W740 R783 R959 W780 W834 W361 W782 R544 W690 W400 W527 W438 W272";

struct PTE
{
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

void sigcont_handler(int signum)
{
    (void)signum; // To avoid unused parameter warning
    waiter = 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <num_pages> <pager_pid>\n", argv[0]);
        exit(1);
    }

    int num_pages = atoi(argv[1]);
    pid_t pager_pid = atoi(argv[argc - 1]);

    struct sigaction sa;
    sa.sa_handler = sigcont_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCONT, &sa, NULL);

    // Open and map the page table
    int fd = open(PAGETABLE, O_RDWR, 0666);
    if (fd == -1)
    {
        perror("Error opening page table file");
        exit(1);
    }
    struct PTE *page_table = mmap(NULL, num_pages * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED)
    {
        perror("Error mapping page table");
        close(fd);
        exit(1);
    }
    printf("\n\n####### MMU #######\n\n");
    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++)
    {
        printf("Page %d ---> valid:%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }

    printf("------------------------------\n");

    const char delim[] = " ";

    char *token = strtok(str, delim);

    while (token != NULL)
    {
        token = strtok(NULL, delim); // Continue to tokenize the string
        if (token == NULL)
            break;
        printf("\n\n####### MMU #######\n\n");
        printf("------------------------------\n");
        char mode = token[0];

        int page = -1;

        int num_len = strlen(token);
        char *page_str = malloc(num_len);
        for (int i = 0; i < num_len - 1; i++)
        {
            if (token[i + 1] == '\0')
                break;
            page_str[i] = token[i + 1];
        }
        page_str[num_len - 1] = '\0';

        page = atoi(page_str);

        if (mode == 'W')
        {
            printf("Write Request for page %d\n", page);
        }
        else
        {
            printf("Read Request for page %d\n", page);
        }

        if (page_table[page].valid)
        {
            hit_count++;
            printf("It is a valid page\n");
            if (mode == 'W')
            {
                page_table[page].dirty = true;
                printf("It is a write request then set the dirty field\n");
            }
        }
        else
        {
            miss_count++;
            printf("It is not a valid page --> page fault\n");
            printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            page_table[page].referenced = getpid();
            kill(pager_pid, SIGUSR1);
            while (/* condition */ waiter)
            {
                /* code */
            }
            waiter = 1;
            printf("MMU resumed by SIGCONT signal from pager\n");
        }

        printf("Page table\n");
        for (int i = 0; i < num_pages; i++)
        {
            printf("Page %d ---> valid:%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
        }

        printf("------------------------------\n\n");
    }
    kill(pager_pid, SIGUSR1);
    munmap(page_table, num_pages * sizeof(struct PTE));
    close(fd);
    double hit_ratio = (double)hit_count / (double)(hit_count + miss_count);
    printf("Hit ratio: %f\n", hit_ratio);
    printf("\nDone all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    printf("MMU terminates.\n");
    return 0;
}