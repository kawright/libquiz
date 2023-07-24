// =============================================================================
// libquiz - The Ultra-Lightweight Test Framework for the C Language
//
// Copyright (C) 2023  Kristoffer A. Wright
// =============================================================================

#include "quiz.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct __QUIZ_QUIZ__ {
    char name[64];
    QUIZ_Outcome outcome;
    int ok;
};

struct __QUIZ_EXAM__ {
    char name[64];
    int skipped;
    int bailed;
    int quizCt;
    QUIZ_Quiz *quizzes;
    int pragmaCt;
    struct __QUIZ_PRAGMA__ **pragmas;
};

struct __QUIZ_PRAGMA__ {
    char name[64];
    int val;
};

QUIZ_Exam QUIZ_newExam(char *name) {
   QUIZ_Exam ret = malloc(sizeof(struct __QUIZ_EXAM__));
    if (ret == NULL) {
        return NULL;
    }
    strncpy(ret->name, name, 63);
    ret->skipped = 0;
    ret->bailed = 0;
    ret->quizCt = 0;
    ret->quizzes = NULL;
    ret->pragmaCt = 0;
    ret->pragmas = NULL;
    return ret;
}

void QUIZ_delExam(QUIZ_Exam exam) {
    for (int i = 0; i < exam->quizCt; i++) {
        free(exam->quizzes[i]);
    }
    free(exam);
}

int QUIZ_run(QUIZ_Exam exam, char *name, int expr, int skip, int todo) {
    if (exam->quizCt == 0) {
        exam->quizzes = malloc(sizeof(struct __QUIZ_QUIZ__));
    } else {
        exam->quizzes = realloc(exam->quizzes, 
            sizeof(struct __QUIZ_QUIZ__) * exam->quizCt+1);
    }
    if (exam->quizzes == NULL) {
        return 0;
    }
    exam->quizCt++;
    strncpy(exam->quizzes[exam->quizCt-1]->name, name, 63);
    if (expr) {
        exam->quizzes[exam->quizCt-1]->ok = 1;
    } else {
        exam->quizzes[exam->quizCt-1]->ok = 0;
    }
    if (skip || exam->skipped) {
        exam->quizzes[exam->quizCt-1]->outcome = QUIZ_Outcome_SKIP;
    } else if (todo) {
        exam->quizzes[exam->quizCt-1]->outcome = QUIZ_Outcome_TODO;
    } else if (exam->quizzes[exam->quizCt-1]->ok) {
        exam->quizzes[exam->quizCt-1]->outcome = QUIZ_Outcome_PASS;
    } else {
        exam->quizzes[exam->quizCt-1]->outcome = QUIZ_Outcome_FAIL;
    }
    return 1;
}

void QUIZ_printResults(QUIZ_Exam exam) {
    printf("TAP Version 14\n");
    for (int i = 0; i < exam->pragmaCt; i++) {
        printf("pragma %s", exam->pragmas[i]->name);
        if (exam->pragmas[i]->val) {
            printf("+\n");
        } else {
            printf("-\n");
        }
    }
    if (exam->skipped) {
        printf("1..0\n");
    } else {
        printf("1..%d\n", exam->quizCt);
    }

    for (int i = 0; i < exam->quizCt; i++) {
        if (exam->quizzes[i]->ok) {
            printf("ok ");
        } else {
            printf("not ok ");
        }
        printf("%d - %s", i+1, exam->quizzes[i]->name);
        if (exam->quizzes[i]->outcome == QUIZ_Outcome_SKIP) {
            printf(" # SKIP");
        }
        if (exam->quizzes[i]->outcome == QUIZ_Outcome_TODO) {
            printf(" # TODO");
        }
        printf("\n");
    }
    if (exam->bailed) {
        printf("Bail Out!\n");
    }
}

void QUIZ_skipAll(QUIZ_Exam exam) {
    exam->skipped = 1;
}

void QUIZ_bailOut(QUIZ_Exam exam) {
    exam->bailed = 1;
    QUIZ_printResults(exam);
    QUIZ_delExam(exam);
    exit(1);
}

int QUIZ_pragma(QUIZ_Exam exam, char *key, int value) {
    if (exam->pragmaCt == 0) {
        exam->pragmas = malloc(sizeof(struct __QUIZ_PRAGMA__));
    } else {
        exam->pragmas = realloc(exam->pragmas, 
            sizeof(struct __QUIZ_PRAGMA__) * exam->pragmaCt+1);
    }
    if (exam->pragmas == NULL) {
        return 0;
    }
    exam->pragmaCt++;
    strncpy(exam->pragmas[exam->pragmaCt-1]->name, key, 63); 
    exam->pragmas[exam->pragmaCt-1]->val = value;
    return 1;
}

char *QUIZ_getExamName(QUIZ_Exam exam) {
    return exam->name;
}

int QUIZ_getTotalCt(QUIZ_Exam exam) {
    return exam->quizCt;
}


int QUIZ_getPassCt(QUIZ_Exam exam) {
    int ret = 0;
    for (int i = 0; i < exam->quizCt; i++) {
        if (exam->quizzes[i]->outcome == QUIZ_Outcome_PASS) {
            ret++;
        }
    }
    return ret;
}

int QUIZ_getFailCt(QUIZ_Exam exam) {
    int ret = 0;
    for (int i = 0; i < exam->quizCt; i++) {
        if (exam->quizzes[i]->outcome == QUIZ_Outcome_FAIL) {
            ret++;
        }
    }
    return ret;
}

int QUIZ_getSkipCt(QUIZ_Exam exam) {
    int ret = 0;
    for (int i = 0; i < exam->quizCt; i++) {
        if (exam->quizzes[i]->outcome == QUIZ_Outcome_SKIP) {
            ret++;
        }
    }
    return ret;
}

int QUIZ_getTodoCt(QUIZ_Exam exam) {
    int ret = 0;
    for (int i = 0; i < exam->quizCt; i++) {
        if (exam->quizzes[i]->outcome == QUIZ_Outcome_TODO) {
            ret++;
        }
    }
    return ret;
}

QUIZ_Quiz QUIZ_getQuiz(QUIZ_Exam exam, int index) {
    if (index < 0 || index >= exam->quizCt) {
        return NULL;
    }
    return exam->quizzes[index];
}

char *QUIZ_getQuizName(QUIZ_Quiz quiz) {
    return quiz->name;
}

QUIZ_Outcome QUIZ_getOutcome(QUIZ_Quiz quiz) {
    return quiz->outcome;
}
