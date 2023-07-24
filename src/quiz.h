// =============================================================================
// libquiz - The Ultra-Lightweight Test Framework for the C Language
//
// Copyright (C) 2023  Kristoffer A. Wright
// =============================================================================

#ifndef __QUIZ_H__
#define __QUIZ_H__

/**
 * Lists all possible outcomes a single quiz run may have.
 */
typedef enum {
    QUIZ_Outcome_PASS,
    QUIZ_Outcome_FAIL,
    QUIZ_Outcome_SKIP,
    QUIZ_Outcome_TODO,
} QUIZ_Outcome;

/**
 * Opaque data used to track multiple quiz runs. Equivalent to a test suite.
 */
typedef struct __QUIZ_EXAM__ *QUIZ_Exam;

/**
 * Opaque data used to store the outcome of a single quiz run.
 */
typedef struct __QUIZ_QUIZ__ *QUIZ_Quiz;

/**
 * Create a new instance of `QUIZ_Exam` with given `name`.
 *
 * Please note that the maximum length for exam names is 63 characters. If
 * `name` exceeds this limit, it will be truncated.
 */
QUIZ_Exam QUIZ_newExam(char *name);

/**
 * Free the heap-memory allocated to `QUIZ_Exam`, including the memory allocated
 * for all quizzes stored therein.
 */
void QUIZ_delExam(QUIZ_Exam exam);

/**
 * Run a quiz. `expr` is the test expression; if it evaluates to any non-zero
 * value, the quiz will pass. Otherwise, it will fail.
 *
 * Either `skip` or `todo` may be given a non-zero value to override the results 
 * with a directive. When both are given, `skip` supercedes `todo`.
 *
 * This function should always return `1`, unless the heap-memory needed to 
 * store the results of the quiz could not be allocated, in which case `0` is 
 * returned to indicate the error condition.
 *
 * Please note that the maximum length for a quiz name is 63 characters--if
 * `name` is larger than this limit, it will be truncated.
 */
int QUIZ_run(QUIZ_Exam exam, char *name, int expr, int skip, int todo);

/**
 * Print the results of all tests run thus far. Printed results are TAP 14
 * compliant.
 */
void QUIZ_printResults(QUIZ_Exam exam);

/**
 * Skip all quizzes on this exam, both before and after this call.
 */
void QUIZ_skipAll(QUIZ_Exam exam);

/**
 * Issue a Bail Out! directive, print the exam results, free the exam, then
 * immediately exit the process with code `1`.
 */
void QUIZ_bailOut(QUIZ_Exam exam);

/**
 * Issue a pragma.
 *
 * This function should always return `1`, unless the heap-memory needed to
 * store the pragma could not be allocated, in which case `0` is returned to
 * indicate the error condition.
 */
int QUIZ_pragma(QUIZ_Exam exam, char *key, int value);

/**
 * Return the name of an exam.
 */
char *QUIZ_getExamName(QUIZ_Exam exam);

/**
 * Get the total number of quizzes run on an exam.
 */
int QUIZ_getTotalCt(QUIZ_Exam exam);

/**
 * Get the number of passed quizzes run on an exam.
 */
int QUIZ_getPassCt(QUIZ_Exam exam);

/**
 * Get the number of failed quizzes run on an exam.
 */
int QUIZ_getFailCt(QUIZ_Exam exam);

/**
 * Get the number of skipped quizzes run on an exam.
 */
int QUIZ_getSkipCt(QUIZ_Exam exam);

/**
 * Get the number of todo-ed quizzes run on an exam.
 */
int QUIZ_getTodoCt(QUIZ_Exam exam);

/**
 * Get the results from a specific quiz, referenced by its index. Quizzes are
 * sorted in the order they were performed, with zero-indexing.
 */
QUIZ_Quiz QUIZ_getQuiz(QUIZ_Exam exam, int index);

/**
 * Get the name of a quiz.
 */
char *QUIZ_getQuizName(QUIZ_Quiz quiz);

/**
 * Get the outcome of a quiz.
 */
QUIZ_Outcome QUIZ_getOutcome(QUIZ_Quiz quiz); 

#endif


