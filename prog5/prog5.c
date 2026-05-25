#include <stdio.h>

union decision {
    unsigned char flags;
    struct {
        int your_choice         : 1;   /* to be computed */
        int your_mothers_choice : 1;
        int your_fathers_choice : 1;
        int socially_acceptable : 1;
        int financially_viable  : 1;
        int do_you_aptitude     : 1;
        int do_you_likeit       : 1;
        int decision            : 1;   /* to be computed */
    } field;
};

/* helper -- just saves me writing this 7 times */
static int ask(const char *q)
{
    int ans = -1;
    while (ans != 0 && ans != 1) {
        printf("  %s (1=yes / 0=no): ", q);
        if (scanf("%d", &ans) != 1) {
            /* flush whatever garbage the user typed */
            while (getchar() != '\n');
            ans = -1;
        }
    }
    return ans;
}

union decision input()
{
    union decision d;
    d.flags = 0;   /* start clean, bitfields can be weird otherwise */

    printf("\n===== Life Decision Evaluator =====\n");
    printf("Be honest. Nobody's watching.\n\n");

    d.field.your_choice         = ask("Is this actually what YOU want?");
    d.field.your_mothers_choice = ask("Would your mother be okay with it?");
    d.field.your_fathers_choice = ask("Would your father be okay with it?");
    d.field.socially_acceptable = ask("Is society going to let you live it down?");
    d.field.financially_viable  = ask("Can you actually afford / sustain this?");
    d.field.do_you_aptitude     = ask("Do you have (or can learn) the skills needed?");
    d.field.do_you_likeit       = ask("Do you genuinely like it, not just the idea of it?");

    return d;
}

/*
 * The logic here is intentionally opinionated:
 *
 *   1. Core trio: your_choice AND likeit AND financially_viable
 *      -- if even one of these is missing, it either isn't yours,
 *         you'll hate it, or you'll go broke. none of those end well.
 *
 *   2. Aptitude: you can learn, but you can't skip this entirely.
 *
 *   3. External approval (parents + society): at least ONE has to be
 *      on your side. living in total opposition is exhausting and
 *      you'll need support eventually.
 *
 * So: GO = core_ok AND aptitude AND (at least 1 external factor).
 */
void make_decision(union decision *d)
{
    int core = d->field.your_choice
             && d->field.do_you_likeit
             && d->field.financially_viable;

    int external_support = d->field.your_mothers_choice
                         + d->field.your_fathers_choice
                         + d->field.socially_acceptable;

    d->field.decision = (core && d->field.do_you_aptitude && external_support >= 1) ? 1 : 0;
}

void print_decision(union decision d)
{
    printf("\n---------- What You Said ----------\n");
    printf("  Your choice?          %s\n", d.field.your_choice         ? "Yes" : "No");
    printf("  Mom approves?         %s\n", d.field.your_mothers_choice ? "Yes" : "No");
    printf("  Dad approves?         %s\n", d.field.your_fathers_choice ? "Yes" : "No");
    printf("  Society ok with it?   %s\n", d.field.socially_acceptable ? "Yes" : "No");
    printf("  Financially viable?   %s\n", d.field.financially_viable  ? "Yes" : "No");
    printf("  Got the aptitude?     %s\n", d.field.do_you_aptitude     ? "Yes" : "No");
    printf("  Actually like it?     %s\n", d.field.do_you_likeit       ? "Yes" : "No");
    printf("-----------------------------------\n");

    if (d.field.decision)
        printf("  >> VERDICT: Go for it. Seriously.\n");
    else
        printf("  >> VERDICT: Maybe pump the brakes here.\n");

    printf("-----------------------------------\n");
}

/*
 * The flags byte is just all 8 bits packed together.
 * Treating it as a plain number gives a rough "alignment score."
 * Higher isn't always better (255 means even the decision bit is set),
 * but popcount of the lower 7 bits tells you how many factors are in your favour.
 *
 * Bit positions (LSB first):
 *   bit0 = your_choice
 *   bit1 = mothers_choice
 *   bit2 = fathers_choice
 *   bit3 = socially_acceptable
 *   bit4 = financially_viable
 *   bit5 = do_you_aptitude
 *   bit6 = do_you_likeit
 *   bit7 = decision  (computed)
 */
void print_conclusion_based_on_flags(union decision d)
{
    /* count set bits in the lower 7 (ignore the computed decision bit) */
    int i, favour = 0;
    unsigned char factors = d.flags & 0x7F;   /* mask off bit7 */
    for (i = 0; i < 7; i++)
        favour += (factors >> i) & 1;

    printf("\n---------- Flag Breakdown ----------\n");
    printf("  Raw flags byte : %3d  (", d.flags);
    for (i = 7; i >= 0; i--) {
        printf("%d", (d.flags >> i) & 1);
        if (i == 7) printf(" ");   /* separate the decision bit visually */
    }
    printf(")\n");
    printf("  Factors in your favour: %d / 7\n", favour);
    printf("------------------------------------\n");

    switch (favour) {
        case 7:
            printf("  Perfect score. Everything lines up.\n"
                   "  Whatever's holding you back is in your head.\n");
            break;
        case 6:
            printf("  One thing's off. Figure out which one and\n"
                   "  decide if you can live with it.\n");
            break;
        case 5:
            printf("  Solid majority. The gaps are real but manageable.\n"
                   "  Make a plan for the two that said no.\n");
            break;
        case 4:
            printf("  Dead even. Classic overthinking territory.\n"
                   "  Sleep on it, then decide fast.\n");
            break;
        case 3:
            printf("  More against than for. This probably\n"
                   "  isn't the right time even if it's the right idea.\n");
            break;
        case 2:
            printf("  Two yeses out of seven is enthusiasm, not a plan.\n"
                   "  Revisit when you've fixed the other five.\n");
            break;
        case 1:
            printf("  One thing going for it. That thing better be\n"
                   "  your_choice, otherwise don't bother.\n");
            break;
        case 0:
            printf("  Zero. Nothing. Walk away.\n"
                   "  Come back with a different question.\n");
            break;
        default:
            /* shouldn't happen but compilers love a missing case */
            printf("  Something went sideways with the bit count.\n");
    }
    printf("------------------------------------\n\n");
}

int main(void)
{
    union decision d = input();
    make_decision(&d);
    print_decision(d);
    print_conclusion_based_on_flags(d);
    return 0;
}