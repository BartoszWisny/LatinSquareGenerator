/**
 * Sudoku Solver API implements logic for solving sudoku boards of different types and sizes using backtracking
 * algorithm, Wave Function Collapse algorithm and Crook's algorithm.
 * @author Bartosz Wiśny
 * @version 1.0
 * @since 2023-07-15
 */

package com.sudoku.SudokuSolver;

import org.json.JSONObject;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.*;

import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * Class contains functions for requests to Sudoku Solver API.
 */

@CrossOrigin
@RestController
public class SudokuSolverAPI {
    /** Stores available Sudoku Solver API paths. */

    public static HashSet<String> SudokuSolverAPIPaths = new HashSet<>(Arrays.asList("/", "/IsValidBoard",
        "/SolveUsingBacktrackingAlgorithm", "/SolveUsingWaveFunctionCollapseAlgorithm", "/SolveUsingCrookAlgorithm",
        "/GetNumberOfSolutions")).stream().sorted().collect(Collectors.toCollection(LinkedHashSet::new));

    /** Defines result field for JSON response. */

    public static String JSON_RESULT = "result";

    /** Defines info field for JSON response. */

    public static String JSON_INFO = "info";

    /** Defines value for result success. */

    public static String JSON_RESULT_SUCCESS = "SUCCESS";

    /** Defines value for result error. */

    public static String JSON_RESULT_ERROR = "ERROR";

    /** Defines class storing sudoku board size, sudoku block width and sudoku block height. */

    class SudokuBoardSizeAndBlockSize {
        /** Stores sudoku board size. */

        int SudokuBoardSize;

        /** Stores sudoku block width. */

        int SudokuBoardBlockWidth;

        /** Stores sudoku block height. */

        int SudokuBoardBlockHeight;

        /**
         * Defines constructor for SudokuBoardSizeAndBlockSize class.
         * @param SudokuBoardSize Parameter is sudoku board size.
         * @param SudokuBoardBlockWidth Parameter is sudoku block width
         * @param SudokuBoardBlockHeight Parameter is sudoku block height.
         */

        SudokuBoardSizeAndBlockSize(int SudokuBoardSize, int SudokuBoardBlockWidth, int SudokuBoardBlockHeight) {
            this.SudokuBoardSize = SudokuBoardSize;
            this.SudokuBoardBlockWidth = SudokuBoardBlockWidth;
            this.SudokuBoardBlockHeight = SudokuBoardBlockHeight;
        }
    }

    /** Stores available sudoku board sizes. */

    private HashSet<SudokuBoardSizeAndBlockSize> SudokuBoardSizeAndBlockSizes = new HashSet<>(Arrays.asList(
        new SudokuBoardSizeAndBlockSize(4, 2, 2),
        new SudokuBoardSizeAndBlockSize(6, 3, 2),
        new SudokuBoardSizeAndBlockSize(9, 3, 3),
        new SudokuBoardSizeAndBlockSize(12, 4, 3),
        new SudokuBoardSizeAndBlockSize(16, 4, 4)));

    /** Defines class storing sudoku type with relevant sudoku board sizes. */

    class SudokuTypeAndSizes {
        /** Stores sudoku type. */

        String SudokuType;

        /** Stores sudoku board sizes for given sudoku type. */

        HashSet<Integer> SudokuBoardSizes;

        /**
         * Defines constructor for SudokuTypeAndSizes class.
         * @param SudokuType Parameter is sudoku type.
         * @param SudokuBoardSizes Parameter is set of sudoku board sizes for given sudoku type.
         */

        SudokuTypeAndSizes(String SudokuType, HashSet<Integer> SudokuBoardSizes) {
            this.SudokuType = SudokuType;
            this.SudokuBoardSizes = SudokuBoardSizes;
        }
    }

    /** Stores available sudoku types. */

    private HashSet<SudokuTypeAndSizes> SudokuTypesAndSizes = new HashSet<>(Arrays.asList(
        new SudokuTypeAndSizes("Anti-Knight", new HashSet<>(Arrays.asList(4, 6, 9, 12, 16))),
        new SudokuTypeAndSizes("Classic", new HashSet<>(Arrays.asList(4, 6, 9, 12, 16))),
        new SudokuTypeAndSizes("Diagonal", new HashSet<>(Arrays.asList(4, 6, 9, 12, 16))),
        new SudokuTypeAndSizes("Offset", new HashSet<>(Arrays.asList(4, 6, 9, 12, 16))),
        new SudokuTypeAndSizes("Non-Consecutive", new HashSet<>(Arrays.asList(6, 9, 12, 16))),
        new SudokuTypeAndSizes("Windoku", new HashSet<>(Arrays.asList(4, 9, 16))),
        new SudokuTypeAndSizes("Center Dot", new HashSet<>(Arrays.asList(9)))));

    /** Stores available knight moves (needed for Anti-Knight sudoku type). */

    private int[][] KnightMoves = new int[][]{{-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, -1}, {2, 1}, {-1, -2}, {1, -2}};

    /** Stores available directions of neighbors of sudoku field (needed for Non-Consecutive sudoku type). */

    private int[][] DirectionsOfNeighbors = new int[][]{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    /** Defines class storing sudoku field row and sudoku field column. */

    class SudokuField {
        /** Stores sudoku field row. */

        int SudokuFieldRow;

        /** Stores sudoku field column. */

        int SudokuFieldColumn;

        /**
         * Defines constructor for SudokuField class.
         * @param SudokuFieldRow Parameter is sudoku field row.
         * @param SudokuFieldColumn Parameter is sudoku field column.
         */

        SudokuField(int SudokuFieldRow, int SudokuFieldColumn) {
            this.SudokuFieldRow = SudokuFieldRow;
            this.SudokuFieldColumn = SudokuFieldColumn;
        }
    }

    /** Defines class storing not filled sudoku fields with minimum number of remaining numbers (not filled sudoku
    fields with minimum number of all possible numbers to fill these sudoku fields) and minimum number of remaining
    numbers for not filled sudoku fields (minimum number of all possible numbers to fill not filled sudoku fields). */

    class NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers {
        /** Stores not filled sudoku fields with minimum number of remaining numbers (not filled sudoku fields with
        minimum number of all possible numbers to fill these sudoku fields). */

        HashSet<SudokuField> NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers;

        /** Stores minimum number of remaining numbers for not filled sudoku fields (minimum number of all possible
        numbers to fill not filled sudoku fields). */

        int MinimumNumberOfRemainingNumbersForNotFilledSudokuFields;

        /**
         * Defines constructor for NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers class.
         * @param NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers Parameter is set of not filled sudoku fields
         * with minimum number of remaining numbers.
         * @param MinimumNumberOfRemainingNumbersForNotFilledSudokuFields Parameter is minimum number of remaining
         * numbers for not filled sudoku fields.
         */

        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(
            HashSet<SudokuField> NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers,
            int MinimumNumberOfRemainingNumbersForNotFilledSudokuFields) {
            this.NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                = NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers;
            this.MinimumNumberOfRemainingNumbersForNotFilledSudokuFields
                = MinimumNumberOfRemainingNumbersForNotFilledSudokuFields;
        }
    }

    /** Defines default value for not filled sudoku field. */

    public static int DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE = 0;

    /** Defines default value. */

    public static int DEFAULT_VALUE = -1;

    /** Defines class storing updated sudoku fields after filling sudoku field and remaining numbers for just filled
    sudoku field (all possible numbers to fill just filled sudoku field - needed in case of backtracking). */

    class UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField {
        /** Stores updated sudoku fields after filling sudoku field. */

        HashSet<SudokuField> UpdatedSudokuFields;

        /** Stores remaining numbers for just filled sudoku field (all possible numbers to fill just filled sudoku
        field). */

        HashSet<Integer> RemainingNumbersForFilledSudokuField;

        /**
         * Defines constructor for UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField class.
         * @param UpdatedSudokuFields Parameter is set of updated sudoku fields after filling sudoku field.
         * @param RemainingNumbersForFilledSudokuField Parameter is set of remaining numbers for just filled sudoku
         * field.
         */

        UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField(HashSet<SudokuField> UpdatedSudokuFields,
            HashSet<Integer> RemainingNumbersForFilledSudokuField) {
            this.UpdatedSudokuFields = UpdatedSudokuFields;
            this.RemainingNumbersForFilledSudokuField = new HashSet<>(RemainingNumbersForFilledSudokuField);
        }
    }

    /** Defines class storing preemptive set - remaining numbers for sudoku fields forming preemptive set and sudoku
    fields forming preemptive set. */

    class PreemptiveSet {
        /** Stores remaining numbers for sudoku fields forming preemptive set. */

        HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSet;

        /** Stores sudoku fields forming preemptive set. */

        HashSet<SudokuField> SudokuFieldsFormingPreemptiveSet;

        /**
         * Defines constructor for PreemptiveSet class.
         * @param RemainingNumbersForSudokuFieldsFormingPreemptiveSet Parameter is set of remaining numbers for sudoku
         * fields forming preemptive set.
         * @param SudokuFieldsFormingPreemptiveSet Parameter is set of sudoku fields forming preemptive set.
         */

        PreemptiveSet(HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSet,
            HashSet<SudokuField> SudokuFieldsFormingPreemptiveSet) {
            this.RemainingNumbersForSudokuFieldsFormingPreemptiveSet
                = RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
            this.SudokuFieldsFormingPreemptiveSet = SudokuFieldsFormingPreemptiveSet;
        }
    }

    /** Defines class storing updated sudoku field and deleted number for updated sudoku field. */

    class UpdatedSudokuFieldAndDeletedNumberForSudokuField {
        /** Stores updated sudoku field. */

        SudokuField UpdatedSudokuField;

        /** Stores deleted number for updated sudoku field. */

        int DeletedNumberForSudokuField;

        /**
         * Defines constructor for UpdatedSudokuFieldAndDeletedNumberForSudokuField class.
         * @param UpdatedSudokuField Parameter is updated sudoku field.
         * @param DeletedNumberForSudokuField Parameter is deleted number for updated sudoku field.
         */

        UpdatedSudokuFieldAndDeletedNumberForSudokuField(SudokuField UpdatedSudokuField,
            int DeletedNumberForSudokuField) {
            this.UpdatedSudokuField = UpdatedSudokuField;
            this.DeletedNumberForSudokuField = DeletedNumberForSudokuField;
        }
    }

    /** Stores number of solutions of sudoku board for Wave Function Collapse algorithm. */

    int NumberOfSudokuSolutionsUsingWaveFunctionCollapseAlgorithm = 0;

    /** Stores number of solutions of sudoku board for Crook's algorithm. */

    int NumberOfSudokuSolutionsUsingCrookAlgorithm = 0;

    /** Defines default value for limit for number of solutions of sudoku board. */

    int GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT = 100;

    /**
     * Shows welcome message.
     * Usage: /
     * @return Returns welcome message.
     */

    @GetMapping(path = "/", produces = MediaType.APPLICATION_JSON_VALUE)
    public String ShowWelcomeMessage() {
        ExecutorService ExecutorService = Executors.newSingleThreadExecutor();

        Callable<String> ShowWelcomeMessageCallable = () -> {
            JSONObject WelcomeMessage = new JSONObject();
            WelcomeMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
            WelcomeMessage.put(JSON_INFO, "Welcome to the Sudoku Solver API website! ");
            return WelcomeMessage.toString();
        };

        Future<String> ShowWelcomeMessageFuture = ExecutorService.submit(ShowWelcomeMessageCallable);

        try {
            String SuccessMessage = ShowWelcomeMessageFuture.get(5, TimeUnit.SECONDS);
            ExecutorService.shutdown();
            return SuccessMessage;
        } catch (InterruptedException | ExecutionException Exception) {
            ShowWelcomeMessageFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Error during execution! ");
            return ErrorMessage.toString();
        } catch (TimeoutException Exception) {
            ShowWelcomeMessageFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Timeout! ");
            return ErrorMessage.toString();
        }
    }

    /**
     * Prints values stored in given hashset.
     * @param Set Parameter is hashset which values should be printed.
     * @return Returns values stored in given hashset.
     */

    public static String PrintHashSet(HashSet Set) {
        Iterator SetIterator = Set.iterator();
        StringBuilder SetValues = new StringBuilder();

        while (SetIterator.hasNext()) {
            SetValues.append(SetIterator.next());

            if (SetIterator.hasNext()) {
                SetValues.append(", ");
            }
        }

        return SetValues.toString();
    }

    /**
     * Checks if SudokuBoardSizeAndBlockSizes contains given sudoku board size.
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @return Returns if SudokuBoardSizeAndBlockSizes contains given sudoku board size.
     */

    private boolean CheckIfContainsSudokuBoardSize(int SudokuBoardSize) {
        for (SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize : SudokuBoardSizeAndBlockSizes) {
            if (SudokuBoardSizeAndBlockSize.SudokuBoardSize == SudokuBoardSize) {
                return true;
            }
        }

        return false;
    }

    /**
     * Checks if SudokuTypesAndSizes contains given sudoku type for given sudoku board size.
     * @param SudokuType Parameter is sudoku type.
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @return Returns if SudokuTypesAndSizes contains given sudoku type for given sudoku board size.
     */

    private boolean CheckIfContainsSudokuTypeForSudokuBoardSize(String SudokuType, int SudokuBoardSize) {
        for (SudokuTypeAndSizes SudokuTypeAndSizes : SudokuTypesAndSizes) {
            if (SudokuTypeAndSizes.SudokuType.equals(SudokuType)
                && SudokuTypeAndSizes.SudokuBoardSizes.contains(SudokuBoardSize)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Prints sizes of sudoku boards.
     * @return Returns sizes of sudoku boards.
     */

    private String PrintSudokuBoardSizes() {
        HashSet<Integer> SudokuBoardSizes = new HashSet<>();

        for (SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize : SudokuBoardSizeAndBlockSizes) {
            SudokuBoardSizes.add(SudokuBoardSizeAndBlockSize.SudokuBoardSize);
        }

        return PrintHashSet(SudokuBoardSizes.stream().sorted().collect(Collectors.toCollection(LinkedHashSet::new)));
    }

    /**
     * Prints types of sudoku and sudoku board sizes for given sudoku type.
     * @return Returns types of sudoku and sudoku board sizes for given sudoku type.
     */

    private String PrintSudokuTypes() {
        Iterator SudokuTypesAndSizesIterator = SudokuTypesAndSizes.stream()
            .sorted(Comparator.comparing(SudokuTypeAndSizes -> SudokuTypeAndSizes.SudokuType)).iterator();
        StringBuilder SudokuTypes = new StringBuilder();

        while (SudokuTypesAndSizesIterator.hasNext()) {
            SudokuTypeAndSizes SudokuTypeAndSizes = (SudokuTypeAndSizes) SudokuTypesAndSizesIterator.next();
            SudokuTypes.append(SudokuTypeAndSizes.SudokuType);
            SudokuTypes.append(" (for SudokuBoardSize parameter values: " + PrintHashSet(
                SudokuTypeAndSizes.SudokuBoardSizes.stream().sorted().collect(
                Collectors.toCollection(LinkedHashSet::new))) + ")");

            if (SudokuTypesAndSizesIterator.hasNext()) {
                SudokuTypes.append(", ");
            }
        }

        return SudokuTypes.toString();
    }

    /**
     * Gets SudokuBoardSizeAndBlockSize object from SudokuBoardSizeAndBlockSizes by given sudoku board size.
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @return Returns SudokuBoardSizeAndBlockSize object found by given sudoku board size.
     */

    private SudokuBoardSizeAndBlockSize GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(int SudokuBoardSize) {
        for (SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize : SudokuBoardSizeAndBlockSizes) {
            if (SudokuBoardSizeAndBlockSize.SudokuBoardSize == SudokuBoardSize) {
                return SudokuBoardSizeAndBlockSize;
            }
        }

        return new SudokuBoardSizeAndBlockSize(DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE);
    }

    /**
     * Gets valid sudoku board format for given sudoku board size.
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @return Returns valid sudoku board format for given sudoku board size.
     */

    private String GetValidSudokuBoardFormat(int SudokuBoardSize) {
        return ("X".repeat(SudokuBoardSize) + "-").repeat(SudokuBoardSize - 1) + "X".repeat(SudokuBoardSize);
    }

    /**
     * Gets sudoku board format rules for given sudoku board size.
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @return Returns sudoku board format rules for given sudoku board size.
     */

    private String GetSudokuBoardFormatRules(int SudokuBoardSize) {
        String SudokuRules = "X should be number between 1 and " + SudokuBoardSize;

        if (SudokuBoardSize > 9) {
            SudokuRules += " (numbers greater than 9 should be written as consecutive capital letters of the alphabet,"
                + " e.g. 10 should be written as A, 11 as B, etc.)";
        }

        SudokuRules += " or X should be _ (underscore) if given sudoku board field is empty";
        return SudokuRules;
    }

    /**
     * Checks if given sudoku board is sudoku solution.
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @param SudokuBoard Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is sudoku solution.
     */

    private boolean CheckIfGivenSudokuBoardIsSudokuSolution(int SudokuBoardSize, String SudokuBoard) {
        for (int Iterator = 0; Iterator < SudokuBoardSize * (SudokuBoardSize + 1) - 1; Iterator++) {
            if (SudokuBoard.charAt(Iterator) == '_') {
                return false;
            }
        }

        return true;
    }

    /**
     * Checks if given sudoku board format is valid.
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @param SudokuBoard Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board format is valid.
     */

    private boolean CheckIfGivenSudokuBoardFormatIsValid(int SudokuBoardSize, String SudokuBoard)
        throws NumberFormatException {
        if (SudokuBoard.length() != SudokuBoardSize * (SudokuBoardSize + 1) - 1) {
            return false;
        }

        for (int Iterator = 1; Iterator < SudokuBoardSize; Iterator++) {
            if (SudokuBoard.charAt(Iterator * (SudokuBoardSize + 1) - 1) != '-') {
                return false;
            }
        }

        boolean IsSudokuSolution = CheckIfGivenSudokuBoardIsSudokuSolution(SudokuBoardSize, SudokuBoard);

        for (int Iterator = 0; Iterator < SudokuBoardSize * (SudokuBoardSize + 1) - 1; Iterator++) {
            char SudokuBoardFormatChar = SudokuBoard.charAt(Iterator);

            if (SudokuBoardFormatChar != '-') {
                if (IsSudokuSolution || SudokuBoardFormatChar != '_') {
                    try {
                        Integer.parseInt(String.valueOf(SudokuBoardFormatChar), SudokuBoardSize + 1);
                    } catch (NumberFormatException Exception) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    /**
     * Converts sudoku board written as string to 2D array of integers.
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @param SudokuBoard Parameter is sudoku board to be converted to 2D array of integers.
     * @return Returns sudoku board converted to 2D array of integers.
     */

    private int[][] SudokuBoardTo2DArrayOfIntegers(int SudokuBoardSize, String SudokuBoard) {
        int[][] SudokuBoardArray = new int[SudokuBoardSize][SudokuBoardSize];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardSize; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardSize; Iterator2++) {
                char SudokuBoardChar = SudokuBoard.charAt(0);

                if (SudokuBoardChar == '_') {
                    SudokuBoardArray[Iterator1][Iterator2] = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
                } else {
                    SudokuBoardArray[Iterator1][Iterator2] = Integer.parseInt(String.valueOf(SudokuBoardChar),
                        SudokuBoardSize + 1);
                }

                SudokuBoard = SudokuBoard.substring(1);
            }

            if (!SudokuBoard.equals("")) {
                SudokuBoard = SudokuBoard.substring(1);
            }
        }

        return SudokuBoardArray;
    }

    /**
     * Gets numbers in rows (numbers that are present in rows).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array of sets containing numbers that are present in relevant rows.
     */

    private HashSet<Integer>[] GetNumbersInRows(int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInRows = new HashSet[SudokuBoardArray.length];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            HashSet<Integer> NumbersInRow = new HashSet<>();

            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                int SudokuBoardNumber = SudokuBoardArray[Iterator1][Iterator2];

                if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NumbersInRow.add(SudokuBoardNumber);
                }
            }

            NumbersInRows[Iterator1] = NumbersInRow;
        }

        return NumbersInRows;
    }

    /**
     * Gets number of non-default numbers in rows.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array containing number of non-default numbers in relevant rows.
     */

    private int[] GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInRows(int[][] SudokuBoardArray) {
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInRows = new int[SudokuBoardArray.length];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            int NumberOfNonDefaultSudokuFieldNotFilledNumbersInRow = 0;

            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                if (SudokuBoardArray[Iterator1][Iterator2] != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NumberOfNonDefaultSudokuFieldNotFilledNumbersInRow++;
                }
            }

            NumberOfNonDefaultSudokuFieldNotFilledNumbersInRows[Iterator1]
                = NumberOfNonDefaultSudokuFieldNotFilledNumbersInRow;
        }

        return NumberOfNonDefaultSudokuFieldNotFilledNumbersInRows;
    }

    /**
     * Checks if given sudoku board is valid in rows (checks if every row of sudoku board does not contain repeating
     * numbers).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in rows.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInRows(int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInRows = GetNumbersInRows(SudokuBoardArray);
        int[] NumbersOfNonDefaultSudokuFieldNotFilledNumbersInRows
            = GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInRows(SudokuBoardArray);

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            if (NumbersInRows[Iterator].size() != NumbersOfNonDefaultSudokuFieldNotFilledNumbersInRows[Iterator]) {
                return false;
            }
        }

        return true;
    }

    /**
     * Gets numbers in columns (numbers that are present in columns).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array of sets containing numbers that are present in relevant columns.
     */

    private HashSet<Integer>[] GetNumbersInColumns(int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInColumns = new HashSet[SudokuBoardArray.length];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            HashSet<Integer> NumbersInColumn = new HashSet<>();

            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                int SudokuBoardNumber = SudokuBoardArray[Iterator2][Iterator1];

                if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NumbersInColumn.add(SudokuBoardNumber);
                }
            }

            NumbersInColumns[Iterator1] = NumbersInColumn;
        }

        return NumbersInColumns;
    }

    /**
     * Gets number of non-default numbers in columns.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array containing number of non-default numbers in relevant columns.
     */

    private int[] GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns(int[][] SudokuBoardArray) {
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns = new int[SudokuBoardArray.length];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            int NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumn = 0;

            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                if (SudokuBoardArray[Iterator2][Iterator1] != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumn++;
                }
            }

            NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns[Iterator1]
                = NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumn;
        }

        return NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns;
    }

    /**
     * Checks if given sudoku board is valid in columns (checks if every column of sudoku board does not contain
     * repeating numbers).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in columns.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInColumns(int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInColumns = GetNumbersInColumns(SudokuBoardArray);
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns
            = GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns(SudokuBoardArray);

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            if (NumbersInColumns[Iterator].size() != NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns[Iterator]) {
                return false;
            }
        }

        return true;
    }

    /**
     * Gets numbers in blocks (numbers that are present in blocks).
     * @param BlocksRows Parameter is number of rows of blocks in sudoku board.
     * @param BlocksColumns Parameter is number of columns of blocks in sudoku board.
     * @param SudokuBoardBlockHeight Parameter is height of block in sudoku board.
     * @param SudokuBoardBlockWidth Parameter is width of block in sudoku board.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array of sets containing numbers that are present in relevant blocks.
     */

    private HashSet<Integer>[] GetNumbersInBlocks(int BlocksRows, int BlocksColumns,
        int SudokuBoardBlockHeight, int SudokuBoardBlockWidth, int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInBlocks = new HashSet[BlocksRows * BlocksColumns];

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                HashSet<Integer> NumbersInBlock = new HashSet<>();

                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                        int SudokuBoardNumber = SudokuBoardArray[Iterator1 * SudokuBoardBlockHeight + Iterator3]
                            [Iterator2 * SudokuBoardBlockWidth + Iterator4];

                        if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NumbersInBlock.add(SudokuBoardNumber);
                        }
                    }
                }

                NumbersInBlocks[Iterator1 * BlocksColumns + Iterator2] = NumbersInBlock;
            }
        }

        return NumbersInBlocks;
    }

    /**
     * Gets number of non-default numbers in blocks.
     * @param BlocksRows Parameter is number of rows of blocks in sudoku board.
     * @param BlocksColumns Parameter is number of columns of blocks in sudoku board.
     * @param SudokuBoardBlockHeight Parameter is height of block in sudoku board.
     * @param SudokuBoardBlockWidth Parameter is width of block in sudoku board.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array containing number of non-default numbers in relevant blocks.
     */

    private int[] GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInBlocks(int BlocksRows, int BlocksColumns,
        int SudokuBoardBlockHeight, int SudokuBoardBlockWidth, int[][] SudokuBoardArray) {
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInBlocks = new int[BlocksRows * BlocksColumns];

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                int NumberOfNonDefaultSudokuFieldNotFilledNumbersInBlock = 0;

                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                        if (SudokuBoardArray[Iterator1 * SudokuBoardBlockHeight + Iterator3]
                            [Iterator2 * SudokuBoardBlockWidth + Iterator4] != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NumberOfNonDefaultSudokuFieldNotFilledNumbersInBlock++;
                        }
                    }
                }

                NumberOfNonDefaultSudokuFieldNotFilledNumbersInBlocks[Iterator1 * BlocksColumns + Iterator2]
                    = NumberOfNonDefaultSudokuFieldNotFilledNumbersInBlock;
            }
        }

        return NumberOfNonDefaultSudokuFieldNotFilledNumbersInBlocks;
    }

    /**
     * Checks if given sudoku board is valid in blocks (checks if every block of sudoku board does not contain repeating
     * numbers).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in blocks.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInBlocks(int[][] SudokuBoardArray) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int BlocksRows = SudokuBoardArray.length / SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        HashSet<Integer>[] NumbersInBlocks = GetNumbersInBlocks(BlocksRows, BlocksColumns,
            SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight, SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth,
            SudokuBoardArray);
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns
            = GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInBlocks(BlocksRows, BlocksColumns,
            SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight, SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth,
            SudokuBoardArray);

        for (int Iterator = 0; Iterator < BlocksRows * BlocksColumns; Iterator++) {
            if (NumbersInBlocks[Iterator].size() != NumberOfNonDefaultSudokuFieldNotFilledNumbersInColumns[Iterator]) {
                return false;
            }
        }

        return true;
    }

    /**
     * Gets numbers in diagonals (numbers that are present in diagonals).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array of sets containing numbers that are present in relevant diagonals.
     */

    private HashSet<Integer>[] GetNumbersInDiagonals(int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInDiagonals = new HashSet[2];
        HashSet<Integer> NumbersInDiagonal1 = new HashSet<>();

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            int SudokuBoardNumber = SudokuBoardArray[Iterator][Iterator];

            if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                NumbersInDiagonal1.add(SudokuBoardNumber);
            }
        }

        NumbersInDiagonals[0] = NumbersInDiagonal1;
        HashSet<Integer> NumbersInDiagonal2 = new HashSet<>();

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            int SudokuBoardNumber = SudokuBoardArray[Iterator][SudokuBoardArray.length - Iterator - 1];

            if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                NumbersInDiagonal2.add(SudokuBoardNumber);
            }
        }

        NumbersInDiagonals[1] = NumbersInDiagonal2;
        return NumbersInDiagonals;
    }

    /**
     * Gets number of non-default numbers in diagonals.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array containing number of non-default numbers in relevant diagonals.
     */

    private int[] GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonals(int[][] SudokuBoardArray) {
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonals = new int[2];
        int NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonal1 = 0;

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            if (SudokuBoardArray[Iterator][Iterator] != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonal1++;
            }
        }

        NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonals[0]
            = NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonal1;
        int NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonal2 = 0;

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            if (SudokuBoardArray[Iterator][SudokuBoardArray.length - Iterator - 1]
                != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonal2++;
            }
        }

        NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonals[1]
            = NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonal2;
        return NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonals;
    }

    /**
     * Checks if given sudoku board is valid in diagonals (checks if every diagonal of sudoku board does not contain
     * repeating numbers).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in diagonals.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInDiagonals(int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInDiagonals = GetNumbersInDiagonals(SudokuBoardArray);
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonals
            = GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonals(SudokuBoardArray);

        for (int Iterator = 0; Iterator < 2; Iterator++) {
            if (NumbersInDiagonals[Iterator].size() != NumberOfNonDefaultSudokuFieldNotFilledNumbersInDiagonals
                [Iterator]) {
                return false;
            }
        }

        return true;
    }

    /**
     * Gets numbers in offset regions (numbers that are present in offset regions).
     * @param BlocksRows Parameter is number of rows of blocks in sudoku board.
     * @param BlocksColumns Parameter is number of columns of blocks in sudoku board.
     * @param SudokuBoardBlockHeight Parameter is height of block in sudoku board.
     * @param SudokuBoardBlockWidth Parameter is width of block in sudoku board.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array of sets containing numbers that are present in relevant offset regions.
     */

    private HashSet<Integer>[] GetNumbersInOffsetRegions(int BlocksRows, int BlocksColumns,
        int SudokuBoardBlockHeight, int SudokuBoardBlockWidth, int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInOffsetRegions = new HashSet[SudokuBoardBlockWidth * SudokuBoardBlockHeight];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                HashSet<Integer> NumbersInOffsetRegion = new HashSet<>();

                for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                        int SudokuBoardNumber = SudokuBoardArray[Iterator3 * SudokuBoardBlockHeight + Iterator1]
                            [Iterator4 * SudokuBoardBlockWidth + Iterator2];

                        if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NumbersInOffsetRegion.add(SudokuBoardNumber);
                        }
                    }
                }

                NumbersInOffsetRegions[Iterator1 * SudokuBoardBlockWidth + Iterator2] = NumbersInOffsetRegion;
            }
        }

        return NumbersInOffsetRegions;
    }

    /**
     * Gets number of non-default numbers in offset regions.
     * @param BlocksRows Parameter is number of rows of blocks in sudoku board.
     * @param BlocksColumns Parameter is number of columns of blocks in sudoku board.
     * @param SudokuBoardBlockHeight Parameter is height of block in sudoku board.
     * @param SudokuBoardBlockWidth Parameter is width of block in sudoku board.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array containing number of non-default numbers in relevant offset regions.
     */

    private int[] GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegions(int BlocksRows, int BlocksColumns,
        int SudokuBoardBlockHeight, int SudokuBoardBlockWidth, int[][] SudokuBoardArray) {
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegions = new int[SudokuBoardBlockWidth
            * SudokuBoardBlockHeight];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                int NumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegion = 0;

                for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                        if (SudokuBoardArray[Iterator3 * SudokuBoardBlockHeight + Iterator1][Iterator4
                            * SudokuBoardBlockWidth + Iterator2] != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegion++;
                        }
                    }
                }

                NumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegions[Iterator1 * SudokuBoardBlockWidth
                    + Iterator2] = NumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegion;
            }
        }

        return NumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegions;
    }

    /**
     * Checks if given sudoku board is valid in offset regions (checks if every offset region of sudoku board does not
     * contain repeating numbers).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in offset regions.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInOffsetRegions(int[][] SudokuBoardArray) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int BlocksRows = SudokuBoardArray.length / SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        HashSet<Integer>[] NumbersInOffsetRegions = GetNumbersInOffsetRegions(BlocksRows, BlocksColumns,
            SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight, SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth,
            SudokuBoardArray);
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegions
            = GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegions(BlocksRows, BlocksColumns,
            SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight, SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth,
            SudokuBoardArray);

        for (int Iterator = 0; Iterator < BlocksRows * BlocksColumns; Iterator++) {
            if (NumbersInOffsetRegions[Iterator].size() != NumberOfNonDefaultSudokuFieldNotFilledNumbersInOffsetRegions
                [Iterator]) {
                return false;
            }
        }

        return true;
    }

    /**
     * Gets numbers in windoku regions (numbers that are present in windoku regions).
     * @param BlocksRowsMinusOne Parameter is number of rows of blocks in sudoku board decreased by one.
     * @param BlocksColumnsMinusOne Parameter is number of columns of blocks in sudoku board decreased by one.
     * @param SudokuBoardBlockHeight Parameter is height of block in sudoku board.
     * @param SudokuBoardBlockWidth Parameter is width of block in sudoku board.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array of sets containing numbers that are present in relevant windoku regions.
     */

    private HashSet<Integer>[] GetNumbersInWindokuRegions(int BlocksRowsMinusOne, int BlocksColumnsMinusOne,
        int SudokuBoardBlockHeight, int SudokuBoardBlockWidth, int[][] SudokuBoardArray) {
        HashSet<Integer>[] NumbersInWindokuRegions = new HashSet[BlocksRowsMinusOne * BlocksColumnsMinusOne];

        for (int Iterator1 = 0; Iterator1 < BlocksRowsMinusOne; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumnsMinusOne; Iterator2++) {
                HashSet<Integer> NumbersInWindokuRegion = new HashSet<>();

                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                        int SudokuBoardNumber = SudokuBoardArray
                            [Iterator1 * (SudokuBoardBlockHeight + 1) + Iterator3 + 1]
                            [Iterator2 * (SudokuBoardBlockWidth + 1) + Iterator4 + 1];

                        if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NumbersInWindokuRegion.add(SudokuBoardNumber);
                        }
                    }
                }

                NumbersInWindokuRegions[Iterator1 * BlocksColumnsMinusOne + Iterator2] = NumbersInWindokuRegion;
            }
        }

        return NumbersInWindokuRegions;
    }

    /**
     * Gets number of non-default numbers in windoku regions.
     * @param BlocksRowsMinusOne Parameter is number of rows of blocks in sudoku board decreased by one.
     * @param BlocksColumnsMinusOne Parameter is number of columns of blocks in sudoku board decreased by one.
     * @param SudokuBoardBlockHeight Parameter is height of block in sudoku board.
     * @param SudokuBoardBlockWidth Parameter is width of block in sudoku board.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns array containing number of non-default numbers in relevant windoku regions.
     */

    private int[] GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegions(int BlocksRowsMinusOne,
        int BlocksColumnsMinusOne, int SudokuBoardBlockHeight, int SudokuBoardBlockWidth, int[][] SudokuBoardArray) {
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegions = new int[BlocksRowsMinusOne
            * BlocksColumnsMinusOne];

        for (int Iterator1 = 0; Iterator1 < BlocksRowsMinusOne; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumnsMinusOne; Iterator2++) {
                int NumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegion = 0;

                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                        if(SudokuBoardArray[Iterator1 * (SudokuBoardBlockHeight + 1) + Iterator3 + 1]
                            [Iterator2 * (SudokuBoardBlockWidth + 1) + Iterator4 + 1]
                            != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegion++;
                        }
                    }
                }

                NumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegions
                    [Iterator1 * BlocksColumnsMinusOne + Iterator2]
                    = NumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegion;
            }
        }

        return NumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegions;
    }

    /**
     * Checks if given sudoku board is valid in windoku regions (checks if windoku regions of sudoku board does not
     * contain repeating numbers).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in windoku regions.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInWindokuRegions(int[][] SudokuBoardArray) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int BlocksRows = SudokuBoardArray.length / SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRowsMinusOne = BlocksRows - 1;
        int BlocksColumnsMinusOne = BlocksColumns - 1;
        HashSet<Integer>[] NumbersInWindokuRegions = GetNumbersInWindokuRegions(BlocksRowsMinusOne,
            BlocksColumnsMinusOne, SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight,
            SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth, SudokuBoardArray);
        int[] NumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegions
            = GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegions(BlocksRowsMinusOne,
            BlocksColumnsMinusOne, SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight,
            SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth, SudokuBoardArray);

        for (int Iterator = 0; Iterator < BlocksRowsMinusOne * BlocksColumnsMinusOne; Iterator++) {
            if (NumbersInWindokuRegions[Iterator].size()
                != NumberOfNonDefaultSudokuFieldNotFilledNumbersInWindokuRegions[Iterator]) {
                return false;
            }
        }

        return true;
    }

    /**
     * Gets numbers in center dot region (numbers that are present in center dot region).
     * @param BlocksRows Parameter is number of rows of blocks in sudoku board.
     * @param BlocksColumns Parameter is number of columns of blocks in sudoku board.
     * @param SudokuBoardBlockHeight Parameter is height of block in sudoku board.
     * @param SudokuBoardBlockWidth Parameter is width of block in sudoku board.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns set containing numbers that are present in center dot region.
     */

    private HashSet<Integer> GetNumbersInCenterDotRegion(int BlocksRows, int BlocksColumns, int SudokuBoardBlockHeight,
        int SudokuBoardBlockWidth, int[][] SudokuBoardArray) {
        int BlockCenterRow = SudokuBoardBlockHeight / 2;
        int BlockCenterColumn = SudokuBoardBlockWidth / 2;
        HashSet<Integer> NumbersInCenterDotRegion = new HashSet<>();

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                int SudokuBoardNumber = SudokuBoardArray[Iterator1 * SudokuBoardBlockHeight + BlockCenterRow]
                    [Iterator2 * SudokuBoardBlockWidth + BlockCenterColumn];

                if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NumbersInCenterDotRegion.add(SudokuBoardNumber);
                }
            }
        }

        return NumbersInCenterDotRegion;
    }

    /**
     * Gets number of non-default numbers in center dot region.
     * @param BlocksRows Parameter is number of rows of blocks in sudoku board.
     * @param BlocksColumns Parameter is number of columns of blocks in sudoku board.
     * @param SudokuBoardBlockHeight Parameter is height of block in sudoku board.
     * @param SudokuBoardBlockWidth Parameter is width of block in sudoku board.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns number of non-default numbers in center dot region.
     */

    private int GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInCenterDotRegion(int BlocksRows, int BlocksColumns,
        int SudokuBoardBlockHeight, int SudokuBoardBlockWidth,int[][] SudokuBoardArray) {
        int BlockCenterRow = SudokuBoardBlockHeight / 2;
        int BlockCenterColumn = SudokuBoardBlockWidth / 2;
        int NumberOfNonDefaultSudokuFieldNotFilledNumbersInCenterDotRegion = 0;

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                if (SudokuBoardArray[Iterator1 * SudokuBoardBlockHeight + BlockCenterRow]
                    [Iterator2 * SudokuBoardBlockWidth + BlockCenterColumn] != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NumberOfNonDefaultSudokuFieldNotFilledNumbersInCenterDotRegion++;
                }
            }
        }

        return NumberOfNonDefaultSudokuFieldNotFilledNumbersInCenterDotRegion;
    }

    /**
     * Checks if given sudoku board is valid in center dot region (checks if center dot region of sudoku board does not
     * contain repeating numbers).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in center dot region.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInCenterDotRegion(int[][] SudokuBoardArray) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int BlocksRows = SudokuBoardArray.length / SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        HashSet<Integer> NumbersInCenterDotRegion = GetNumbersInCenterDotRegion(BlocksRows, BlocksColumns,
            SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight, SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth,
            SudokuBoardArray);
        int NumberOfNonDefaultSudokuFieldNotFilledNumbersInCenterDotRegion
            = GetNumberOfNonDefaultSudokuFieldNotFilledNumbersInCenterDotRegion(BlocksRows, BlocksColumns,
            SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight, SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth,
            SudokuBoardArray);

        if (NumbersInCenterDotRegion.size() != NumberOfNonDefaultSudokuFieldNotFilledNumbersInCenterDotRegion) {
            return false;
        }

        return true;
    }

    /**
     * Checks if given sudoku board is valid in Anti-Knight type (checks for every sudoku field if every sudoku field
     * away from given sudoku field by knight's move does not contain same number as given sudoku field).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in Anti-Knight type.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInAntiKnightType(int[][] SudokuBoardArray) {
        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                int SudokuBoardNumber = SudokuBoardArray[Iterator1][Iterator2];

                if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    for (int Iterator3 = 0; Iterator3 < KnightMoves.length; Iterator3++) {
                        int SudokuFieldSudokuFieldRow = Iterator1 + KnightMoves[Iterator3][0];
                        int SudokuFieldSudokuFieldColumn = Iterator2 + KnightMoves[Iterator3][1];

                        if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow < SudokuBoardArray.length
                            && SudokuFieldSudokuFieldColumn >= 0 && SudokuFieldSudokuFieldColumn
                            < SudokuBoardArray.length) {
                            if (SudokuBoardArray[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                == SudokuBoardNumber) {
                                return false;
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

    /**
     * Checks if given sudoku board is valid in Non-Consecutive type (checks for every sudoku field if every sudoku
     * field adjacent to given sudoku field does not contain number from given sudoku field decreased by one or number
     * from given sudoku field increased by one).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is valid in Non-Consecutive type.
     */

    private boolean CheckIfGivenSudokuBoardIsValidInNonConsecutiveType(int[][] SudokuBoardArray) {
        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                int SudokuBoardNumber = SudokuBoardArray[Iterator1][Iterator2];

                if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    for (int Iterator3 = 0; Iterator3 < DirectionsOfNeighbors.length; Iterator3++) {
                        int SudokuFieldSudokuFieldRow = Iterator1 + DirectionsOfNeighbors[Iterator3][0];
                        int SudokuFieldSudokuFieldColumn = Iterator2 + DirectionsOfNeighbors[Iterator3][1];

                        if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow < SudokuBoardArray.length
                            && SudokuFieldSudokuFieldColumn >= 0 && SudokuFieldSudokuFieldColumn
                            < SudokuBoardArray.length) {
                            if (SudokuBoardArray[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                                int SudokuBoardNumberMinusOne = SudokuBoardNumber - 1;
                                int SudokuBoardNumberPlusOne = SudokuBoardNumber + 1;

                                if (SudokuBoardArray[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                    == SudokuBoardNumberMinusOne || SudokuBoardArray[SudokuFieldSudokuFieldRow]
                                    [SudokuFieldSudokuFieldColumn] == SudokuBoardNumberPlusOne) {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

    /**
     * Checks if given sudoku board is valid for given sudoku type.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns if given sudoku board is valid for given sudoku type.
     */

    private boolean CheckIfGivenSudokuBoardIsValid(int[][] SudokuBoardArray, String SudokuType) {
        if (Thread.currentThread().isInterrupted()) {
            throw new RuntimeException();
        }

        boolean IsGivenSudokuBoardValid = CheckIfGivenSudokuBoardIsValidInRows(SudokuBoardArray)
            && CheckIfGivenSudokuBoardIsValidInColumns(SudokuBoardArray)
            && CheckIfGivenSudokuBoardIsValidInBlocks(SudokuBoardArray);

        switch (SudokuType) {
            case "Anti-Knight":
                IsGivenSudokuBoardValid = IsGivenSudokuBoardValid && CheckIfGivenSudokuBoardIsValidInAntiKnightType(
                    SudokuBoardArray);
                break;
            case "Diagonal":
                IsGivenSudokuBoardValid = IsGivenSudokuBoardValid && CheckIfGivenSudokuBoardIsValidInDiagonals(
                    SudokuBoardArray);
                break;
            case "Non-Consecutive":
                IsGivenSudokuBoardValid = IsGivenSudokuBoardValid && CheckIfGivenSudokuBoardIsValidInNonConsecutiveType(
                    SudokuBoardArray);
                break;
            case "Offset":
                IsGivenSudokuBoardValid = IsGivenSudokuBoardValid && CheckIfGivenSudokuBoardIsValidInOffsetRegions(
                    SudokuBoardArray);
                break;
            case "Windoku":
                IsGivenSudokuBoardValid = IsGivenSudokuBoardValid && CheckIfGivenSudokuBoardIsValidInWindokuRegions(
                    SudokuBoardArray);
                break;
            case "Center Dot":
                IsGivenSudokuBoardValid = IsGivenSudokuBoardValid && CheckIfGivenSudokuBoardIsValidInCenterDotRegion(
                    SudokuBoardArray);
                break;
        }

        return IsGivenSudokuBoardValid;
    }

    /**
     * Checks if given sudoku board is valid for given sudoku type.
     * Usage: /IsValidBoard?SudokuBoardSize={SudokuBoardSize}&amp;SudokuType={SudokuType}&amp;SudokuBoard={SudokuBoard}
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @param SudokuType Parameter is sudoku type.
     * @param SudokuBoard Parameter is sudoku board to be checked.
     * @return Returns information about correctness of given sudoku board for given sudoku type.
     */

    @GetMapping(path = "/IsValidBoard", produces = MediaType.APPLICATION_JSON_VALUE)
    public String CheckIfGivenSudokuBoardIsValid(@RequestParam int SudokuBoardSize, @RequestParam String SudokuType,
        @RequestParam String SudokuBoard) {
        ExecutorService ExecutorService = Executors.newSingleThreadExecutor();

        Callable<String> CheckIfGivenSudokuBoardIsValidCallable = () -> {
            if (!CheckIfContainsSudokuBoardSize(SudokuBoardSize)
                || !CheckIfContainsSudokuTypeForSudokuBoardSize(SudokuType, SudokuBoardSize)
                || !CheckIfGivenSudokuBoardFormatIsValid(SudokuBoardSize, SudokuBoard)) {
                JSONObject ErrorMessage = new JSONObject();
                ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                String ErrorMessageInfo = "";

                if (!CheckIfContainsSudokuBoardSize(SudokuBoardSize)) {
                    ErrorMessageInfo += "Invalid SudokuBoardSize parameter value! Available SudokuBoardSize parameter"
                        + " values are: " + PrintSudokuBoardSizes() + ". ";
                }

                if (!CheckIfContainsSudokuTypeForSudokuBoardSize(SudokuType, SudokuBoardSize)) {
                    ErrorMessageInfo += "Invalid SudokuType parameter value for given SudokuBoardSize parameter value!"
                        + " Available SudokuType parameter values are: " + PrintSudokuTypes() + ". ";
                }

                if (CheckIfContainsSudokuBoardSize(SudokuBoardSize)
                    && CheckIfContainsSudokuTypeForSudokuBoardSize(SudokuType, SudokuBoardSize)
                    && !CheckIfGivenSudokuBoardFormatIsValid(SudokuBoardSize, SudokuBoard)) {
                    ErrorMessageInfo += "Invalid SudokuBoard format! Valid format for given SudokuBoardSize: "
                        + SudokuBoardSize + " and SudokuType: " + SudokuType + " is: "
                        + GetValidSudokuBoardFormat(SudokuBoardSize) + " where: "
                        + GetSudokuBoardFormatRules(SudokuBoardSize) + ". ";
                }

                ErrorMessage.put(JSON_INFO, ErrorMessageInfo);
                return ErrorMessage.toString();
            }

            int[][] SudokuBoardArray = SudokuBoardTo2DArrayOfIntegers(SudokuBoardSize, SudokuBoard);

            if (CheckIfGivenSudokuBoardIsValid(SudokuBoardArray, SudokuType)) {
                JSONObject SuccessMessage = new JSONObject();
                SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                SuccessMessage.put(JSON_INFO, "Valid sudoku board! ");
                return SuccessMessage.toString();
            }

            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Invalid sudoku board! ");
            return ErrorMessage.toString();
        };

        Future<String> CheckIfGivenSudokuBoardIsValidFuture = ExecutorService.submit(
            CheckIfGivenSudokuBoardIsValidCallable);

        try {
            String SuccessMessage = CheckIfGivenSudokuBoardIsValidFuture.get(10, TimeUnit.SECONDS);
            ExecutorService.shutdown();
            return SuccessMessage;
        } catch (InterruptedException | ExecutionException Exception) {
            CheckIfGivenSudokuBoardIsValidFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Error during execution! ");
            return ErrorMessage.toString();
        } catch (TimeoutException Exception) {
            CheckIfGivenSudokuBoardIsValidFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Timeout! ");
            return ErrorMessage.toString();
        }
    }

    /**
     * Converts sudoku board to string compatible with sudoku board format.
     * @param SudokuBoardArray Parameter is sudoku board to be converted.
     * @return Returns sudoku board converted to string compatible with sudoku board format.
     */

    private String SudokuBoardToString(int[][] SudokuBoardArray) {
        StringBuilder SudokuBoard = new StringBuilder();

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                SudokuBoard.append(Integer.toString(SudokuBoardArray[Iterator1][Iterator2],
                    SudokuBoardArray.length + 1).toUpperCase());
            }

            if (Iterator1 != SudokuBoardArray.length - 1) {
                SudokuBoard.append("-");
            }
        }

        return SudokuBoard.toString();
    }

    /**
     * Gets first sudoku field on sudoku board which has not been filled yet.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns first sudoku field on sudoku board which has not been filled yet.
     */

    private SudokuField GetFirstNotFilledSudokuField(int[][] SudokuBoardArray) {
        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                if (SudokuBoardArray[Iterator1][Iterator2] == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    return new SudokuField(Iterator1, Iterator2);
                }
            }
        }

        return new SudokuField(DEFAULT_VALUE, DEFAULT_VALUE);
    }

    /**
     * Gets remaining numbers for first not filled sudoku field (all possible numbers to fill first sudoku field on
     * sudoku board which has not been filled yet).
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @param FirstNotFilledSudokuFieldSudokuFieldRow Parameter is row of first not filled sudoku field.
     * @param FirstNotFilledSudokuFieldSudokuFieldColumn Parameter is column of first not filled sudoku field.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns set containing remaining numbers for first not filled sudoku field.
     */

    private HashSet<Integer> GetRemainingNumbersForNotFilledSudokuField(int[][] SudokuBoardArray,
        int FirstNotFilledSudokuFieldSudokuFieldRow, int FirstNotFilledSudokuFieldSudokuFieldColumn,
        String SudokuType) {
        if (SudokuBoardArray[FirstNotFilledSudokuFieldSudokuFieldRow][FirstNotFilledSudokuFieldSudokuFieldColumn]
            != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
            return new HashSet<>();
        }

        HashSet<Integer> RemainingNumbersForSudokuField = new HashSet<>(IntStream
            .rangeClosed(1, SudokuBoardArray.length).boxed().collect(Collectors.toList()));

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            int SudokuBoardNumber = SudokuBoardArray[FirstNotFilledSudokuFieldSudokuFieldRow][Iterator];

            if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
            }
        }

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            int SudokuBoardNumber = SudokuBoardArray[Iterator][FirstNotFilledSudokuFieldSudokuFieldColumn];

            if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
            }
        }

        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int FirstNotFilledSudokuFieldSudokuFieldBlockRow = FirstNotFilledSudokuFieldSudokuFieldRow
            / SudokuBoardBlockHeight;
        int FirstNotFilledSudokuFieldSudokuFieldBlockColumn = FirstNotFilledSudokuFieldSudokuFieldColumn
            / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                int SudokuBoardNumber = SudokuBoardArray[FirstNotFilledSudokuFieldSudokuFieldBlockRow
                    * SudokuBoardBlockHeight + Iterator1][FirstNotFilledSudokuFieldSudokuFieldBlockColumn
                    * SudokuBoardBlockWidth + Iterator2];

                if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
                }
            }
        }

        int BlocksRows = SudokuBoardArray.length / SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardBlockWidth;

        switch (SudokuType) {
            case "Anti-Knight":
                for (int Iterator = 0; Iterator < KnightMoves.length; Iterator++) {
                    int SudokuFieldSudokuFieldRow = FirstNotFilledSudokuFieldSudokuFieldRow + KnightMoves[Iterator][0];
                    int SudokuFieldSudokuFieldColumn = FirstNotFilledSudokuFieldSudokuFieldColumn
                        + KnightMoves[Iterator][1];

                    if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow < SudokuBoardArray.length
                        && SudokuFieldSudokuFieldColumn >= 0 && SudokuFieldSudokuFieldColumn
                        < SudokuBoardArray.length) {
                        int SudokuBoardNumber = SudokuBoardArray[SudokuFieldSudokuFieldRow]
                            [SudokuFieldSudokuFieldColumn];

                        if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
                        }
                    }
                }

                break;
            case "Diagonal":
                if (FirstNotFilledSudokuFieldSudokuFieldRow == FirstNotFilledSudokuFieldSudokuFieldColumn) {
                    for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
                        int SudokuBoardNumber = SudokuBoardArray[Iterator][Iterator];

                        if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
                        }
                    }
                }

                if (FirstNotFilledSudokuFieldSudokuFieldRow + FirstNotFilledSudokuFieldSudokuFieldColumn
                    == SudokuBoardArray.length - 1) {
                    for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
                        int SudokuBoardNumber = SudokuBoardArray[Iterator][SudokuBoardArray.length - Iterator - 1];

                        if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
                        }
                    }
                }

                break;
            case "Non-Consecutive":
                for (int Iterator = 0; Iterator < DirectionsOfNeighbors.length; Iterator++) {
                    int SudokuFieldSudokuFieldRow = FirstNotFilledSudokuFieldSudokuFieldRow
                        + DirectionsOfNeighbors[Iterator][0];
                    int SudokuFieldSudokuFieldColumn = FirstNotFilledSudokuFieldSudokuFieldColumn
                        + DirectionsOfNeighbors[Iterator][1];

                    if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow < SudokuBoardArray.length
                        && SudokuFieldSudokuFieldColumn >= 0 && SudokuFieldSudokuFieldColumn
                        < SudokuBoardArray.length) {
                        int SudokuBoardNumber = SudokuBoardArray[SudokuFieldSudokuFieldRow]
                            [SudokuFieldSudokuFieldColumn];

                        if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            int SudokuBoardNumberMinusOne = SudokuBoardNumber - 1;
                            int SudokuBoardNumberPlusOne = SudokuBoardNumber + 1;
                            RemainingNumbersForSudokuField.remove(SudokuBoardNumberMinusOne);
                            RemainingNumbersForSudokuField.remove(SudokuBoardNumberPlusOne);
                        }
                    }
                }

                break;
            case "Offset":
                int SudokuFieldSudokuFieldRowInBlock = FirstNotFilledSudokuFieldSudokuFieldRow % SudokuBoardBlockHeight;
                int SudokuFieldSudokuFieldColumnInBlock = FirstNotFilledSudokuFieldSudokuFieldColumn
                    % SudokuBoardBlockWidth;

                for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
                    for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                        int SudokuBoardNumber = SudokuBoardArray[Iterator1 * SudokuBoardBlockHeight
                            + SudokuFieldSudokuFieldRowInBlock][Iterator2 * SudokuBoardBlockWidth
                            + SudokuFieldSudokuFieldColumnInBlock];

                        if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
                        }
                    }
                }

                break;
            case "Windoku":
                int SudokuBoardBlockHeightPlusOne = SudokuBoardBlockHeight + 1;
                int SudokuBoardBlockWidthPlusOne = SudokuBoardBlockWidth + 1;

                if (FirstNotFilledSudokuFieldSudokuFieldRow % SudokuBoardBlockHeightPlusOne != 0
                    && FirstNotFilledSudokuFieldSudokuFieldColumn % SudokuBoardBlockWidthPlusOne != 0) {
                    int FirstNotFilledSudokuFieldSudokuFieldWindokuRegionRow = FirstNotFilledSudokuFieldSudokuFieldRow
                        / SudokuBoardBlockHeightPlusOne;
                    int FirstNotFilledSudokuFieldSudokuFieldWindokuRegionColumn
                        = FirstNotFilledSudokuFieldSudokuFieldColumn / SudokuBoardBlockWidthPlusOne;

                    for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
                        for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                            int SudokuBoardNumber = SudokuBoardArray
                                [FirstNotFilledSudokuFieldSudokuFieldWindokuRegionRow * SudokuBoardBlockHeightPlusOne
                                + Iterator1 + 1][FirstNotFilledSudokuFieldSudokuFieldWindokuRegionColumn
                                * SudokuBoardBlockWidthPlusOne + Iterator2 + 1];

                            if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                                RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
                            }
                        }
                    }
                }

                break;
            case "Center Dot":
                int BlockCenterRow = SudokuBoardBlockHeight / 2;
                int BlockCenterColumn = SudokuBoardBlockWidth / 2;

                if (FirstNotFilledSudokuFieldSudokuFieldRow % SudokuBoardBlockHeight == BlockCenterRow
                    && FirstNotFilledSudokuFieldSudokuFieldColumn % SudokuBoardBlockWidth == BlockCenterColumn) {
                    for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
                        for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                            int SudokuBoardNumber = SudokuBoardArray[Iterator1 * SudokuBoardBlockHeight
                                + BlockCenterRow][Iterator2 * SudokuBoardBlockWidth + BlockCenterColumn];

                            if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                                RemainingNumbersForSudokuField.remove(SudokuBoardNumber);
                            }
                        }
                    }
                }

                break;
        }

        return RemainingNumbersForSudokuField;
    }

    // long CountCheckedSudokuStatesForBacktrackingAlgorithm = 0;

    /**
     * Solves given sudoku board using backtracking algorithm.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns if given sudoku board has been solved.
     */

    private boolean SolveSudokuBoardUsingBacktrackingAlgorithm(int[][] SudokuBoardArray, String SudokuType) {
        if (Thread.currentThread().isInterrupted()) {
            throw new RuntimeException();
        }

        SudokuField FirstNotFilledSudokuField = GetFirstNotFilledSudokuField(SudokuBoardArray);
        int FirstNotFilledSudokuFieldSudokuFieldRow = FirstNotFilledSudokuField.SudokuFieldRow;
        int FirstNotFilledSudokuFieldSudokuFieldColumn = FirstNotFilledSudokuField.SudokuFieldColumn;

        if (FirstNotFilledSudokuFieldSudokuFieldRow == DEFAULT_VALUE
            && FirstNotFilledSudokuFieldSudokuFieldColumn == DEFAULT_VALUE) {
            return true;
        }

        HashSet<Integer> RemainingNumbersForFirstNotFilledSudokuField = GetRemainingNumbersForNotFilledSudokuField(
            SudokuBoardArray, FirstNotFilledSudokuFieldSudokuFieldRow, FirstNotFilledSudokuFieldSudokuFieldColumn,
            SudokuType);

        for (int Iterator = 0; Iterator < RemainingNumbersForFirstNotFilledSudokuField.size(); Iterator++) {
            SudokuBoardArray[FirstNotFilledSudokuFieldSudokuFieldRow][FirstNotFilledSudokuFieldSudokuFieldColumn]
                = (int) RemainingNumbersForFirstNotFilledSudokuField.toArray()[Iterator];

            // CountCheckedSudokuStatesForBacktrackingAlgorithm++;

            if (SolveSudokuBoardUsingBacktrackingAlgorithm(SudokuBoardArray, SudokuType)) {
                return true;
            }

            SudokuBoardArray[FirstNotFilledSudokuFieldSudokuFieldRow][FirstNotFilledSudokuFieldSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
        }

        return false;
    }

    /**
     * Solves given sudoku board using backtracking algorithm.
     * Usage: /SolveUsingBacktrackingAlgorithm?SudokuBoardSize={SudokuBoardSize}&amp;SudokuType={SudokuType}
     * &amp;SudokuBoard={SudokuBoard}
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @param SudokuType Parameter is sudoku type.
     * @param SudokuBoard Parameter is sudoku board to be solved.
     * @return Returns information about solution of sudoku board obtained by using backtracking algorithm.
     */

    @GetMapping(path = "/SolveUsingBacktrackingAlgorithm", produces = MediaType.APPLICATION_JSON_VALUE)
    public String SolveSudokuBoardUsingBacktrackingAlgorithm(@RequestParam int SudokuBoardSize,
        @RequestParam String SudokuType, @RequestParam String SudokuBoard) {
        ExecutorService ExecutorService = Executors.newSingleThreadExecutor();

        Callable<String> SolveSudokuBoardUsingBacktrackingAlgorithmCallable = () -> {
            String CheckIfGivenSudokuBoardIsValid = CheckIfGivenSudokuBoardIsValid(SudokuBoardSize, SudokuType,
                SudokuBoard);
            JSONObject CheckIfGivenSudokuBoardIsValidMessage = new JSONObject(CheckIfGivenSudokuBoardIsValid);
            String CheckIfGivenSudokuBoardIsValidMessageResult = CheckIfGivenSudokuBoardIsValidMessage.getString(
                JSON_RESULT);

            if (CheckIfGivenSudokuBoardIsValidMessageResult.equals(JSON_RESULT_SUCCESS)) {
                int[][] SudokuBoardArray = SudokuBoardTo2DArrayOfIntegers(SudokuBoardSize, SudokuBoard);

                // long StartTime = System.nanoTime();
                // CountCheckedSudokuStatesForBacktrackingAlgorithm = 0;

                if (SolveSudokuBoardUsingBacktrackingAlgorithm(SudokuBoardArray, SudokuType)) {
                    // long StopTime = System.nanoTime();
                    // System.out.println((double) (StopTime - StartTime) / 1000000000);
                    // System.out.println(CountCheckedSudokuStatesForBacktrackingAlgorithm);

                    String SolvedSudokuBoard = SudokuBoardToString(SudokuBoardArray);
                    String CheckIfGivenSolvedSudokuBoardIsValid = CheckIfGivenSudokuBoardIsValid(SudokuBoardSize,
                        SudokuType, SolvedSudokuBoard);
                    JSONObject CheckIfGivenSolvedSudokuBoardIsValidMessage = new JSONObject(
                        CheckIfGivenSolvedSudokuBoardIsValid);
                    String CheckIfGivenSolvedSudokuBoardIsValidMessageResult
                        = CheckIfGivenSolvedSudokuBoardIsValidMessage.getString(JSON_RESULT);

                    if (CheckIfGivenSolvedSudokuBoardIsValidMessageResult.equals(JSON_RESULT_SUCCESS)) {
                        JSONObject SuccessMessage = new JSONObject();
                        SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                        SuccessMessage.put(JSON_INFO, SolvedSudokuBoard);
                        return SuccessMessage.toString();
                    }

                    JSONObject ErrorMessage = new JSONObject();
                    ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                    ErrorMessage.put(JSON_INFO, "Sudoku board has no solution! ");
                    return ErrorMessage.toString();
                }

                JSONObject ErrorMessage = new JSONObject();
                ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                ErrorMessage.put(JSON_INFO, "Sudoku board has no solution! ");
                return ErrorMessage.toString();
            } else {
                return CheckIfGivenSudokuBoardIsValid;
            }
        };

        Future<String> SolveSudokuBoardUsingBacktrackingAlgorithmFuture = ExecutorService.submit(
                SolveSudokuBoardUsingBacktrackingAlgorithmCallable);

        try {
            String SuccessMessage = SolveSudokuBoardUsingBacktrackingAlgorithmFuture.get(15, TimeUnit.SECONDS);
            ExecutorService.shutdown();
            return SuccessMessage;
        } catch (InterruptedException | ExecutionException Exception) {
            SolveSudokuBoardUsingBacktrackingAlgorithmFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Error during execution! ");
            return ErrorMessage.toString();
        } catch (TimeoutException Exception) {
            SolveSudokuBoardUsingBacktrackingAlgorithmFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Timeout! ");
            return ErrorMessage.toString();
        }
    }

    /**
     * Checks if given sudoku board is solved.
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @return Returns if given sudoku board is solved.
     */

    private boolean CheckIfSudokuBoardIsSolved(int[][] SudokuBoardArray) {
        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                if (SudokuBoardArray[Iterator1][Iterator2] == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * Gets remaining numbers for sudoku fields (all possible numbers to fill sudoku fields).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns array of sets containing remaining numbers for relevant sudoku fields.
     */

    private HashSet<Integer>[][] GetRemainingNumbersForSudokuFields(int[][] SudokuBoardArray, String SudokuType) {
        HashSet<Integer>[][] RemainingNumbersForSudokuFields = new HashSet[SudokuBoardArray.length]
            [SudokuBoardArray.length];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                if (SudokuBoardArray[Iterator1][Iterator2] != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2] = new HashSet<>();
                } else {
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2] = new HashSet<>(IntStream
                        .rangeClosed(1, SudokuBoardArray.length).boxed().collect(Collectors.toList()));
                }
            }
        }

        HashSet<Integer>[] NumbersInRows = GetNumbersInRows(SudokuBoardArray);
        HashSet<Integer>[] NumbersInColumns = GetNumbersInColumns(SudokuBoardArray);

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                RemainingNumbersForSudokuFields[Iterator1][Iterator2].removeAll(NumbersInRows[Iterator1]);
                RemainingNumbersForSudokuFields[Iterator1][Iterator2].removeAll(NumbersInColumns[Iterator2]);
            }
        }

        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = SudokuBoardArray.length / SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardBlockWidth;
        HashSet<Integer>[] NumbersInBlocks = GetNumbersInBlocks(BlocksRows, BlocksColumns, SudokuBoardBlockHeight,
            SudokuBoardBlockWidth, SudokuBoardArray);

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                        RemainingNumbersForSudokuFields[Iterator1 * SudokuBoardBlockHeight + Iterator3]
                            [Iterator2 * SudokuBoardBlockWidth + Iterator4].removeAll(NumbersInBlocks[Iterator1
                            * BlocksColumns + Iterator2]);
                    }
                }
            }
        }

        switch (SudokuType) {
            case "Anti-Knight":
                for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
                    for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                        for (int Iterator3 = 0; Iterator3 < KnightMoves.length; Iterator3++) {
                            int SudokuFieldSudokuFieldRow = Iterator1 + KnightMoves[Iterator3][0];
                            int SudokuFieldSudokuFieldColumn = Iterator2 + KnightMoves[Iterator3][1];

                            if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow < SudokuBoardArray.length
                                && SudokuFieldSudokuFieldColumn >= 0 && SudokuFieldSudokuFieldColumn
                                < SudokuBoardArray.length) {
                                int SudokuBoardNumber = SudokuBoardArray[SudokuFieldSudokuFieldRow]
                                    [SudokuFieldSudokuFieldColumn];

                                if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                                    RemainingNumbersForSudokuFields[Iterator1][Iterator2].remove(SudokuBoardNumber);
                                }
                            }
                        }
                    }
                }

                break;
            case "Diagonal":
                HashSet<Integer>[] NumbersInDiagonals = GetNumbersInDiagonals(SudokuBoardArray);

                for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
                    RemainingNumbersForSudokuFields[Iterator][Iterator].removeAll(NumbersInDiagonals[0]);
                    RemainingNumbersForSudokuFields[Iterator][SudokuBoardArray.length - Iterator - 1].removeAll(
                        NumbersInDiagonals[1]);
                }

                break;
            case "Non-Consecutive":
                for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
                    for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                        for (int Iterator3 = 0; Iterator3 < DirectionsOfNeighbors.length; Iterator3++) {
                            int SudokuFieldSudokuFieldRow = Iterator1 + DirectionsOfNeighbors[Iterator3][0];
                            int SudokuFieldSudokuFieldColumn = Iterator2 + DirectionsOfNeighbors[Iterator3][1];

                            if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow < SudokuBoardArray.length
                                && SudokuFieldSudokuFieldColumn >= 0 && SudokuFieldSudokuFieldColumn
                                < SudokuBoardArray.length) {
                                int SudokuBoardNumber = SudokuBoardArray[SudokuFieldSudokuFieldRow]
                                    [SudokuFieldSudokuFieldColumn];

                                if (SudokuBoardNumber != DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                                    int SudokuBoardNumberMinusOne = SudokuBoardNumber - 1;
                                    int SudokuBoardNumberPlusOne = SudokuBoardNumber + 1;
                                    RemainingNumbersForSudokuFields[Iterator1][Iterator2].remove(
                                        SudokuBoardNumberMinusOne);
                                    RemainingNumbersForSudokuFields[Iterator1][Iterator2].remove(
                                        SudokuBoardNumberPlusOne);
                                }
                            }
                        }
                    }
                }

                break;
            case "Offset":
                HashSet<Integer>[] NumbersInOffsetRegions = GetNumbersInOffsetRegions(BlocksRows, BlocksColumns,
                    SudokuBoardBlockHeight, SudokuBoardBlockWidth, SudokuBoardArray);

                for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
                    for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                        for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                            for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                                RemainingNumbersForSudokuFields[Iterator3 * SudokuBoardBlockHeight + Iterator1]
                                    [Iterator4 * SudokuBoardBlockWidth + Iterator2].removeAll(NumbersInOffsetRegions
                                    [Iterator1 * SudokuBoardBlockWidth + Iterator2]);
                            }
                        }
                    }
                }

                break;
            case "Windoku":
                int BlocksRowsMinusOne = BlocksRows - 1;
                int BlocksColumnsMinusOne = BlocksColumns - 1;
                HashSet<Integer>[] NumbersInWindokuRegions = GetNumbersInWindokuRegions(BlocksRowsMinusOne,
                    BlocksColumnsMinusOne, SudokuBoardBlockHeight, SudokuBoardBlockWidth, SudokuBoardArray);

                for (int Iterator1 = 0; Iterator1 < BlocksRowsMinusOne; Iterator1++) {
                    for (int Iterator2 = 0; Iterator2 < BlocksColumnsMinusOne; Iterator2++) {
                        for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                            for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                                RemainingNumbersForSudokuFields
                                    [Iterator1 * (SudokuBoardBlockHeight + 1) + Iterator3 + 1]
                                    [Iterator2 * (SudokuBoardBlockWidth + 1) + Iterator4 + 1].removeAll(
                                    NumbersInWindokuRegions[Iterator1 * BlocksColumnsMinusOne + Iterator2]);
                            }
                        }
                    }
                }

                break;
            case "Center Dot":
                HashSet<Integer> NumbersInCenterDotRegion = GetNumbersInCenterDotRegion(BlocksRows, BlocksColumns,
                    SudokuBoardBlockHeight, SudokuBoardBlockWidth, SudokuBoardArray);
                int BlockCenterRow = SudokuBoardBlockHeight / 2;
                int BlockCenterColumn = SudokuBoardBlockWidth / 2;

                for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
                    for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                        RemainingNumbersForSudokuFields[Iterator1 * SudokuBoardBlockHeight + BlockCenterRow][Iterator2
                            * SudokuBoardBlockWidth + BlockCenterColumn].removeAll(NumbersInCenterDotRegion);
                    }
                }

                break;
        }

        return RemainingNumbersForSudokuFields;
    }

    /**
     * Gets number of remaining numbers for sudoku fields (number of all possible numbers to fill sudoku fields).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns array containing number of remaining numbers for relevant sudoku fields.
     */

    private int[][] GetNumberOfRemainingNumbersForSudokuFields(HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        int[][] NumberOfRemainingNumbersForSudokuFields = new int[RemainingNumbersForSudokuFields.length]
            [RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                NumberOfRemainingNumbersForSudokuFields[Iterator1][Iterator2] = RemainingNumbersForSudokuFields
                    [Iterator1][Iterator2].size();
            }
        }

        return NumberOfRemainingNumbersForSudokuFields;
    }

    /**
     * Gets all not filled sudoku fields with minimum number of remaining numbers (all not filled sudoku fields with
     * minimum number of all possible numbers to fill these sudoku fields) and minimum number of remaining numbers for
     * not filled sudoku fields (minimum number of all possible numbers to fill not filled sudoku fields).
     * @param SudokuBoardArray Parameter is sudoku board to be checked.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers object containing set of not filled
     * sudoku fields with minimum number of remaining numbers (not filled sudoku fields with minimum number of all
     * possible numbers to fill these sudoku fields) and minimum number of remaining numbers for not filled sudoku
     * fields (minimum number of all possible numbers to fill not filled sudoku fields).
     */

    private NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
        GetNotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(int[][] SudokuBoardArray,
        HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        if (CheckIfSudokuBoardIsSolved(SudokuBoardArray)) {
            return new NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(new HashSet<>(), DEFAULT_VALUE);
        }

        int[][] NumberOfRemainingNumbersForSudokuFields = GetNumberOfRemainingNumbersForSudokuFields(
            RemainingNumbersForSudokuFields);
        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            = new NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(new HashSet<>(), SudokuBoardArray.length);

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                if (SudokuBoardArray[Iterator1][Iterator2] == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    int NumberOfRemainingNumbersForNotFilledSudokuField = NumberOfRemainingNumbersForSudokuFields
                        [Iterator1][Iterator2];
                    int MinimumNumberOfRemainingNumbersForNotFilledSudokuFields
                        = NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                        .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields;

                    if (NumberOfRemainingNumbersForNotFilledSudokuField
                        <= MinimumNumberOfRemainingNumbersForNotFilledSudokuFields) {
                        if (NumberOfRemainingNumbersForNotFilledSudokuField
                            < MinimumNumberOfRemainingNumbersForNotFilledSudokuFields) {
                            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                                .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.clear();
                            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                                .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields
                                = NumberOfRemainingNumbersForNotFilledSudokuField;
                        }

                        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                            .add(new SudokuField(Iterator1, Iterator2));
                    }
                }
            }
        }

        return NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers;
    }

    /**
     * Updates remaining numbers for sudoku fields (all possible numbers to fill sudoku fields).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param FilledSudokuFieldSudokuFieldRow Parameter is row of filled sudoku field.
     * @param FilledSudokuFieldSudokuFieldColumn Parameter is column of filled sudoku field.
     * @param NumberForFilledSudokuField Parameter is number in filled sudoku field.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField object containing updated remaining
     * numbers for sudoku fields and remaining numbers for filled sudoku field.
     */

    private UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField UpdateRemainingNumbersForSudokuFields(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, int FilledSudokuFieldSudokuFieldRow,
        int FilledSudokuFieldSudokuFieldColumn, int NumberForFilledSudokuField, String SudokuType) {
        UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
            = new UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField(new HashSet<>(),
            RemainingNumbersForSudokuFields[FilledSudokuFieldSudokuFieldRow][FilledSudokuFieldSudokuFieldColumn]);
        RemainingNumbersForSudokuFields[FilledSudokuFieldSudokuFieldRow][FilledSudokuFieldSudokuFieldColumn].clear();

        for (int Iterator = 0; Iterator < RemainingNumbersForSudokuFields.length; Iterator++) {
            if (RemainingNumbersForSudokuFields[FilledSudokuFieldSudokuFieldRow][Iterator].remove(
                NumberForFilledSudokuField)) {
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(new SudokuField(
                    FilledSudokuFieldSudokuFieldRow, Iterator));
            }

            if (RemainingNumbersForSudokuFields[Iterator][FilledSudokuFieldSudokuFieldColumn].remove(
                NumberForFilledSudokuField)) {
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(new SudokuField(
                    Iterator, FilledSudokuFieldSudokuFieldColumn));
            }
        }

        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int FilledSudokuFieldSudokuFieldBlockRow = FilledSudokuFieldSudokuFieldRow / SudokuBoardBlockHeight;
        int FilledSudokuFieldSudokuFieldBlockColumn = FilledSudokuFieldSudokuFieldColumn / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                if (RemainingNumbersForSudokuFields[FilledSudokuFieldSudokuFieldBlockRow * SudokuBoardBlockHeight
                    + Iterator1][FilledSudokuFieldSudokuFieldBlockColumn * SudokuBoardBlockWidth + Iterator2].remove(
                    NumberForFilledSudokuField)) {
                    UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(new SudokuField(
                        FilledSudokuFieldSudokuFieldBlockRow * SudokuBoardBlockHeight + Iterator1,
                        FilledSudokuFieldSudokuFieldBlockColumn * SudokuBoardBlockWidth + Iterator2));
                }
            }
        }

        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;

        switch (SudokuType) {
            case "Anti-Knight":
                for (int Iterator = 0; Iterator < KnightMoves.length; Iterator++) {
                    int SudokuFieldSudokuFieldRow = FilledSudokuFieldSudokuFieldRow + KnightMoves[Iterator][0];
                    int SudokuFieldSudokuFieldColumn = FilledSudokuFieldSudokuFieldColumn + KnightMoves[Iterator][1];

                    if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow
                        < RemainingNumbersForSudokuFields.length && SudokuFieldSudokuFieldColumn >= 0
                        && SudokuFieldSudokuFieldColumn < RemainingNumbersForSudokuFields.length) {
                        if (RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                            .remove(NumberForFilledSudokuField)) {
                            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(
                                new SudokuField(SudokuFieldSudokuFieldRow, SudokuFieldSudokuFieldColumn));
                        }
                    }
                }

                break;
            case "Diagonal":
                if (FilledSudokuFieldSudokuFieldRow == FilledSudokuFieldSudokuFieldColumn) {
                    for (int Iterator = 0; Iterator < RemainingNumbersForSudokuFields.length; Iterator++) {
                        if (RemainingNumbersForSudokuFields[Iterator][Iterator].remove(NumberForFilledSudokuField)) {
                            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(
                                new SudokuField(Iterator, Iterator));
                        }
                    }
                }

                if (FilledSudokuFieldSudokuFieldRow + FilledSudokuFieldSudokuFieldColumn
                    == RemainingNumbersForSudokuFields.length - 1) {
                    for (int Iterator = 0; Iterator < RemainingNumbersForSudokuFields.length; Iterator++) {
                        if (RemainingNumbersForSudokuFields[Iterator]
                            [RemainingNumbersForSudokuFields.length - Iterator - 1].remove(
                            NumberForFilledSudokuField)) {
                            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(
                                new SudokuField(Iterator,
                                RemainingNumbersForSudokuFields.length - Iterator - 1));
                        }
                    }
                }

                break;
            case "Offset":
                int SudokuFieldSudokuFieldRowInBlock = FilledSudokuFieldSudokuFieldRow % SudokuBoardBlockHeight;
                int SudokuFieldSudokuFieldColumnInBlock = FilledSudokuFieldSudokuFieldColumn % SudokuBoardBlockWidth;

                for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
                    for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                        if (RemainingNumbersForSudokuFields[Iterator1 * SudokuBoardBlockHeight
                            + SudokuFieldSudokuFieldRowInBlock][Iterator2 * SudokuBoardBlockWidth
                            + SudokuFieldSudokuFieldColumnInBlock].remove(NumberForFilledSudokuField)) {
                            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(
                                new SudokuField(Iterator1 * SudokuBoardBlockHeight
                                + SudokuFieldSudokuFieldRowInBlock, Iterator2 * SudokuBoardBlockWidth
                                + SudokuFieldSudokuFieldColumnInBlock));
                        }
                    }
                }

                break;
            case "Windoku":
                int SudokuBoardBlockHeightPlusOne = SudokuBoardBlockHeight + 1;
                int SudokuBoardBlockWidthPlusOne = SudokuBoardBlockWidth + 1;

                if (FilledSudokuFieldSudokuFieldRow % SudokuBoardBlockHeightPlusOne != 0
                    && FilledSudokuFieldSudokuFieldColumn % SudokuBoardBlockWidthPlusOne != 0) {
                    int FilledSudokuFieldSudokuFieldWindokuRegionRow = FilledSudokuFieldSudokuFieldRow
                        / SudokuBoardBlockHeightPlusOne;
                    int FilledSudokuFieldSudokuFieldWindokuRegionColumn = FilledSudokuFieldSudokuFieldColumn
                        / SudokuBoardBlockWidthPlusOne;

                    for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
                        for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                            if (RemainingNumbersForSudokuFields[FilledSudokuFieldSudokuFieldWindokuRegionRow
                                * SudokuBoardBlockHeightPlusOne + Iterator1 + 1]
                                [FilledSudokuFieldSudokuFieldWindokuRegionColumn * SudokuBoardBlockWidthPlusOne
                                + Iterator2 + 1].remove(NumberForFilledSudokuField)) {
                                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(
                                    new SudokuField(FilledSudokuFieldSudokuFieldWindokuRegionRow
                                    * SudokuBoardBlockHeightPlusOne + Iterator1 + 1,
                                    FilledSudokuFieldSudokuFieldWindokuRegionColumn
                                    * SudokuBoardBlockWidthPlusOne + Iterator2 + 1));
                            }
                        }
                    }
                }

                break;
            case "Center Dot":
                int BlockCenterRow = SudokuBoardBlockHeight / 2;
                int BlockCenterColumn = SudokuBoardBlockWidth / 2;

                if (FilledSudokuFieldSudokuFieldRow % SudokuBoardBlockHeight == BlockCenterRow
                    && FilledSudokuFieldSudokuFieldColumn % SudokuBoardBlockWidth == BlockCenterColumn) {
                    for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
                        for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                            if (RemainingNumbersForSudokuFields[Iterator1 * SudokuBoardBlockHeight + BlockCenterRow]
                                [Iterator2 * SudokuBoardBlockWidth + BlockCenterColumn].remove(
                                NumberForFilledSudokuField)) {
                                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.UpdatedSudokuFields.add(
                                    new SudokuField(Iterator1 * SudokuBoardBlockHeight + BlockCenterRow,
                                    Iterator2 * SudokuBoardBlockWidth + BlockCenterColumn));
                            }
                        }
                    }
                }

                break;
        }

        return UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField;
    }

    /**
     * Updates remaining numbers for sudoku fields (all possible numbers to fill sudoku fields) in Non-Consecutive type.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param FilledSudokuFieldSudokuFieldRow Parameter is row of filled sudoku field.
     * @param FilledSudokuFieldSudokuFieldColumn Parameter is column of filled sudoku field.
     * @param NumberForFilledSudokuField Parameter is number in filled sudoku field.
     * @return Returns set of UpdatedSudokuFieldAndDeletedNumberForSudokuField objects containing updated sudoku field
     * and deleted number for updated sudoku field.
     */

    private HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
        UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        int FilledSudokuFieldSudokuFieldRow, int FilledSudokuFieldSudokuFieldColumn, int NumberForFilledSudokuField) {
        HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField> UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
            = new HashSet<>();

        for (int Iterator = 0; Iterator < DirectionsOfNeighbors.length; Iterator++) {
            int SudokuFieldSudokuFieldRow = FilledSudokuFieldSudokuFieldRow + DirectionsOfNeighbors[Iterator][0];
            int SudokuFieldSudokuFieldColumn = FilledSudokuFieldSudokuFieldColumn + DirectionsOfNeighbors[Iterator][1];

            if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow < RemainingNumbersForSudokuFields.length
                && SudokuFieldSudokuFieldColumn >= 0 && SudokuFieldSudokuFieldColumn
                < RemainingNumbersForSudokuFields.length) {
                int NumberForFilledSudokuFieldMinusOne = NumberForFilledSudokuField - 1;
                int NumberForFilledSudokuFieldPlusOne = NumberForFilledSudokuField + 1;

                if (RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn].remove(
                    NumberForFilledSudokuFieldMinusOne)) {
                    SudokuField UpdatedSudokuField = new SudokuField(SudokuFieldSudokuFieldRow,
                        SudokuFieldSudokuFieldColumn);
                    UpdatedSudokuFieldAndDeletedNumberForSudokuField UpdatedSudokuFieldAndDeletedNumberForSudokuField
                        = new UpdatedSudokuFieldAndDeletedNumberForSudokuField(UpdatedSudokuField,
                        NumberForFilledSudokuFieldMinusOne);
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields.add(
                        UpdatedSudokuFieldAndDeletedNumberForSudokuField);
                }

                if (RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn].remove(
                    NumberForFilledSudokuFieldPlusOne)) {
                    SudokuField UpdatedSudokuField = new SudokuField(SudokuFieldSudokuFieldRow,
                        SudokuFieldSudokuFieldColumn);
                    UpdatedSudokuFieldAndDeletedNumberForSudokuField UpdatedSudokuFieldAndDeletedNumberForSudokuField
                        = new UpdatedSudokuFieldAndDeletedNumberForSudokuField(UpdatedSudokuField,
                        NumberForFilledSudokuFieldPlusOne);
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields.add(
                        UpdatedSudokuFieldAndDeletedNumberForSudokuField);
                }
            }
        }

        return UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields;
    }

    /**
     * Updates remaining numbers for sudoku fields (all possible numbers to fill sudoku fields - needed for
     * backtracking).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField Parameter is
     * UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField object containing updated remaining numbers for sudoku
     * fields and remaining numbers for filled sudoku field.
     * @param FilledSudokuFieldSudokuFieldRow Parameter is row of filled sudoku field.
     * @param FilledSudokuFieldSudokuFieldColumn Parameter is column of filled sudoku field.
     * @param NumberForFilledSudokuField Parameter is number in filled sudoku field.
     */

    private void UpdateRemainingNumbersForSudokuFields(HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField, int FilledSudokuFieldSudokuFieldRow,
        int FilledSudokuFieldSudokuFieldColumn, int NumberForFilledSudokuField) {
        RemainingNumbersForSudokuFields[FilledSudokuFieldSudokuFieldRow][FilledSudokuFieldSudokuFieldColumn]
            .addAll(UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField.RemainingNumbersForFilledSudokuField);
        HashSet<SudokuField> UpdatedSudokuFields = UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
            .UpdatedSudokuFields;

        for (int Iterator = 0; Iterator < UpdatedSudokuFields.size(); Iterator++) {
            SudokuField UpdatedSudokuField = (SudokuField) UpdatedSudokuFields.toArray()[Iterator];
            RemainingNumbersForSudokuFields[UpdatedSudokuField.SudokuFieldRow][UpdatedSudokuField.SudokuFieldColumn]
                .add(NumberForFilledSudokuField);
        }
    }

    /**
     * Updates remaining numbers for sudoku fields (all possible numbers to fill sudoku fields - needed for
     * backtracking) in Non-Consecutive type.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields Parameter is set of
     * UpdatedSudokuFieldAndDeletedNumberForSudokuField objects containing updated sudoku field and deleted number for
     * updated sudoku field.
     */

    private void UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField> UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields) {
        for (int Iterator = 0; Iterator < UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields.size(); Iterator++) {
            UpdatedSudokuFieldAndDeletedNumberForSudokuField UpdatedSudokuFieldAndDeletedNumberForSudokuField
                = (UpdatedSudokuFieldAndDeletedNumberForSudokuField) UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                .toArray()[Iterator];
            SudokuField UpdatedSudokuField = UpdatedSudokuFieldAndDeletedNumberForSudokuField.UpdatedSudokuField;
            int DeletedNumberForSudokuField = UpdatedSudokuFieldAndDeletedNumberForSudokuField
                .DeletedNumberForSudokuField;
            RemainingNumbersForSudokuFields[UpdatedSudokuField.SudokuFieldRow][UpdatedSudokuField.SudokuFieldColumn]
                .add(DeletedNumberForSudokuField);
        }
    }

    // long CountCheckedSudokuStatesForWaveFunctionCollapseAlgorithm = 0;

    /**
     * Solves given sudoku board using Wave Function Collapse algorithm - uniqueness in unit heuristic (with
     * backtracking).
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns if given sudoku board has been solved.
     */

    private boolean SolveSudokuBoardUsingWaveFunctionCollapseAlgorithm(int[][] SudokuBoardArray,
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, String SudokuType) {
        if (Thread.currentThread().isInterrupted()) {
            throw new RuntimeException();
        }

        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            = GetNotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(SudokuBoardArray,
            RemainingNumbersForSudokuFields);

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.isEmpty()
            && NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == DEFAULT_VALUE) {
            return true;
        }

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 0) {
            return false;
        }

        SudokuField NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (SudokuField)
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.toArray()[0];
        int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow
            = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldRow;
        int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn
            = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldColumn;
        HashSet<Integer> RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers
            = RemainingNumbersForSudokuFields[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
            [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn];

        for (int Iterator = 0; Iterator
            < RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.size(); Iterator++) {
            int RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (int)
                RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.toArray()[Iterator];
            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers;
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField = UpdateRemainingNumbersForSudokuFields(
                RemainingNumbersForSudokuFields, NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers, SudokuType);
            HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields = new HashSet<>();

            if (SudokuType.equals("Non-Consecutive")) {
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                    = UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                    RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            }

            // CountCheckedSudokuStatesForWaveFunctionCollapseAlgorithm++;

            if (SolveSudokuBoardUsingWaveFunctionCollapseAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields,
                SudokuType)) {
                return true;
            }

            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);

            if (SudokuType.equals("Non-Consecutive")) {
                UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields);
            }
        }

        return false;
    }

    /**
     * Solves given sudoku board using Wave Function Collapse algorithm (with backtracking).
     * Usage: /SolveUsingWaveFunctionCollapseAlgorithm?SudokuBoardSize={SudokuBoardSize}&amp;SudokuType={SudokuType}
     * &amp;SudokuBoard={SudokuBoard}
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @param SudokuType Parameter is sudoku type.
     * @param SudokuBoard Parameter is sudoku board to be checked.
     * @return Returns information about solution of sudoku board obtained by using Wave Function Collapse algorithm
     * (with backtracking).
     */

    @GetMapping(path = "/SolveUsingWaveFunctionCollapseAlgorithm", produces = MediaType.APPLICATION_JSON_VALUE)
    public String SolveSudokuBoardUsingWaveFunctionCollapseAlgorithm(@RequestParam int SudokuBoardSize,
        @RequestParam String SudokuType, @RequestParam String SudokuBoard) {
        ExecutorService ExecutorService = Executors.newSingleThreadExecutor();

        Callable<String> SolveSudokuBoardUsingWaveFunctionCollapseAlgorithmCallable = () -> {
            String CheckIfGivenSudokuBoardIsValid = CheckIfGivenSudokuBoardIsValid(SudokuBoardSize, SudokuType,
                SudokuBoard);
            JSONObject CheckIfGivenSudokuBoardIsValidMessage = new JSONObject(CheckIfGivenSudokuBoardIsValid);
            String CheckIfGivenSudokuBoardIsValidMessageResult = CheckIfGivenSudokuBoardIsValidMessage.getString(
                JSON_RESULT);

            if (CheckIfGivenSudokuBoardIsValidMessageResult.equals(JSON_RESULT_SUCCESS)) {
                int[][] SudokuBoardArray = SudokuBoardTo2DArrayOfIntegers(SudokuBoardSize, SudokuBoard);
                HashSet<Integer>[][] RemainingNumbersForSudokuFields = GetRemainingNumbersForSudokuFields(
                    SudokuBoardArray, SudokuType);

                // long StartTime = System.nanoTime();
                // CountCheckedSudokuStatesForWaveFunctionCollapseAlgorithm = 0;

                if (SolveSudokuBoardUsingWaveFunctionCollapseAlgorithm(SudokuBoardArray,
                    RemainingNumbersForSudokuFields, SudokuType)) {
                    // long StopTime = System.nanoTime();
                    // System.out.println((double) (StopTime - StartTime) / 1000000000);
                    // System.out.println(CountCheckedSudokuStatesForWaveFunctionCollapseAlgorithm);

                    String SolvedSudokuBoard = SudokuBoardToString(SudokuBoardArray);
                    String CheckIfGivenSolvedSudokuBoardIsValid = CheckIfGivenSudokuBoardIsValid(SudokuBoardSize,
                        SudokuType, SolvedSudokuBoard);
                    JSONObject CheckIfGivenSolvedSudokuBoardIsValidMessage = new JSONObject(
                        CheckIfGivenSolvedSudokuBoardIsValid);
                    String CheckIfGivenSolvedSudokuBoardIsValidMessageResult
                        = CheckIfGivenSolvedSudokuBoardIsValidMessage.getString(JSON_RESULT);

                    if (CheckIfGivenSolvedSudokuBoardIsValidMessageResult.equals(JSON_RESULT_SUCCESS)) {
                        JSONObject SuccessMessage = new JSONObject();
                        SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                        SuccessMessage.put(JSON_INFO, SolvedSudokuBoard);
                        return SuccessMessage.toString();
                    }

                    JSONObject ErrorMessage = new JSONObject();
                    ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                    ErrorMessage.put(JSON_INFO, "Sudoku board has no solution! ");
                    return ErrorMessage.toString();
                }

                JSONObject ErrorMessage = new JSONObject();
                ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                ErrorMessage.put(JSON_INFO, "Sudoku board has no solution! ");
                return ErrorMessage.toString();
            } else {
                return CheckIfGivenSudokuBoardIsValid;
            }
        };

        Future<String> SolveSudokuBoardUsingWaveFunctionCollapseAlgorithmFuture = ExecutorService.submit(
            SolveSudokuBoardUsingWaveFunctionCollapseAlgorithmCallable);

        try {
            String SuccessMessage = SolveSudokuBoardUsingWaveFunctionCollapseAlgorithmFuture.get(15,
                TimeUnit.SECONDS);
            ExecutorService.shutdown();
            return SuccessMessage;
        } catch (InterruptedException | ExecutionException Exception) {
            SolveSudokuBoardUsingWaveFunctionCollapseAlgorithmFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Error during execution! ");
            return ErrorMessage.toString();
        } catch (TimeoutException Exception) {
            SolveSudokuBoardUsingWaveFunctionCollapseAlgorithmFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Timeout! ");
            return ErrorMessage.toString();
        }
    }

    /**
     * Gets sudoku fields with unique remaining numbers in relevant rows (sudoku fields with number not present in other
     * sudoku fields in the same row).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns array of sets containing unique remaining numbers in rows for relevant sudoku fields.
     */

    private HashSet<Integer>[][] GetSudokuFieldsWithUniqueRemainingNumberInRows(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInRows = new HashSet
            [RemainingNumbersForSudokuFields.length][RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < SudokuFieldsWithUniqueRemainingNumberInRows.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuFieldsWithUniqueRemainingNumberInRows.length; Iterator2++) {
                SudokuFieldsWithUniqueRemainingNumberInRows[Iterator1][Iterator2] = new HashSet<>(
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]);
            }
        }

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length; Iterator3++) {
                    if (Iterator2 != Iterator3) {
                        SudokuFieldsWithUniqueRemainingNumberInRows[Iterator1][Iterator2].removeAll(
                            RemainingNumbersForSudokuFields[Iterator1][Iterator3]);
                    }
                }
            }
        }

        return SudokuFieldsWithUniqueRemainingNumberInRows;
    }

    /**
     * Updates remaining numbers for sudoku fields with unique remaining numbers in relevant rows (set for this sudoku
     * field should contain only that unique number and this number should be removed from sets for sudoku fields in the
     * same column, in the same block, etc.).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuFieldsWithUniqueRemainingNumberInRows Parameter is array of sets containing unique remaining numbers
     * in rows for relevant sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     */

    private void UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInRows(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInRows, String SudokuType) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                if (SudokuFieldsWithUniqueRemainingNumberInRows[Iterator1][Iterator2].size() == 1) {
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2] = SudokuFieldsWithUniqueRemainingNumberInRows
                        [Iterator1][Iterator2];
                    int UniqueRemainingNumberInSudokuField = (int) SudokuFieldsWithUniqueRemainingNumberInRows
                        [Iterator1][Iterator2].toArray()[0];

                    for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length; Iterator3++) {
                        if (Iterator1 != Iterator3) {
                            RemainingNumbersForSudokuFields[Iterator3][Iterator2].remove(
                                UniqueRemainingNumberInSudokuField);
                        }
                    }

                    int SudokuFieldSudokuFieldBlockRow = Iterator1 / SudokuBoardBlockHeight;
                    int SudokuFieldSudokuFieldBlockColumn = Iterator2 / SudokuBoardBlockWidth;

                    for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                        for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                            int SudokuFieldSudokuFieldRow = SudokuFieldSudokuFieldBlockRow * SudokuBoardBlockHeight
                                + Iterator3;
                            int SudokuFieldSudokuFieldColumn = SudokuFieldSudokuFieldBlockColumn * SudokuBoardBlockWidth
                                + Iterator4;

                            if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2 != SudokuFieldSudokuFieldColumn) {
                                RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                    .remove(UniqueRemainingNumberInSudokuField);
                            }
                        }
                    }

                    switch (SudokuType) {
                        case "Anti-Knight":
                            for (int Iterator3 = 0; Iterator3 < KnightMoves.length; Iterator3++) {
                                int SudokuFieldSudokuFieldRow = Iterator1 + KnightMoves[Iterator3][0];
                                int SudokuFieldSudokuFieldColumn = Iterator2 + KnightMoves[Iterator3][1];

                                if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow
                                    < RemainingNumbersForSudokuFields.length && SudokuFieldSudokuFieldColumn >= 0
                                    && SudokuFieldSudokuFieldColumn < RemainingNumbersForSudokuFields.length) {
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(UniqueRemainingNumberInSudokuField);
                                }
                            }

                            break;
                        case "Diagonal":
                            if (Iterator1 == Iterator2) {
                                for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length;
                                    Iterator3++) {
                                    if (Iterator1 != Iterator3) {
                                        RemainingNumbersForSudokuFields[Iterator3][Iterator3].remove(
                                            UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            if (Iterator1 + Iterator2 == RemainingNumbersForSudokuFields.length - 1) {
                                for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length;
                                    Iterator3++) {
                                    if (Iterator1 != Iterator3) {
                                        RemainingNumbersForSudokuFields[Iterator3]
                                            [RemainingNumbersForSudokuFields.length - Iterator3 - 1].remove(
                                            UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            break;
                        case "Non-Consecutive":
                            for (int Iterator3 = 0; Iterator3 < DirectionsOfNeighbors.length; Iterator3++) {
                                int SudokuFieldSudokuFieldRow = Iterator1 + DirectionsOfNeighbors[Iterator3][0];
                                int SudokuFieldSudokuFieldColumn = Iterator2 + DirectionsOfNeighbors[Iterator3][1];

                                if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow
                                    < RemainingNumbersForSudokuFields.length && SudokuFieldSudokuFieldColumn >= 0
                                    && SudokuFieldSudokuFieldColumn < RemainingNumbersForSudokuFields.length) {
                                    int UniqueRemainingNumberInSudokuFieldMinusOne
                                        = UniqueRemainingNumberInSudokuField - 1;
                                    int UniqueRemainingNumberInSudokuFieldPlusOne
                                        = UniqueRemainingNumberInSudokuField + 1;
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(
                                        UniqueRemainingNumberInSudokuFieldMinusOne);
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(
                                        UniqueRemainingNumberInSudokuFieldPlusOne);
                                }
                            }

                            break;
                        case "Offset":
                            int SudokuFieldSudokuFieldRowInBlock = Iterator1 % SudokuBoardBlockHeight;
                            int SudokuFieldSudokuFieldColumnInBlock = Iterator2 % SudokuBoardBlockWidth;

                            for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                                for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                                    int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight
                                        + SudokuFieldSudokuFieldRowInBlock;
                                    int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth
                                        + SudokuFieldSudokuFieldColumnInBlock;

                                    if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                        != SudokuFieldSudokuFieldColumn) {
                                        RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                            [SudokuFieldSudokuFieldColumn].remove(UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            break;
                        case "Windoku":
                            int SudokuBoardBlockHeightPlusOne = SudokuBoardBlockHeight + 1;
                            int SudokuBoardBlockWidthPlusOne = SudokuBoardBlockWidth + 1;

                            if (Iterator1 % SudokuBoardBlockHeightPlusOne != 0
                                && Iterator2 % SudokuBoardBlockWidthPlusOne != 0) {
                                int SudokuFieldWindokuRegionRow = Iterator1 / SudokuBoardBlockHeightPlusOne;
                                int SudokuFieldWindokuRegionColumn = Iterator2 / SudokuBoardBlockWidthPlusOne;

                                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                                        int SudokuFieldSudokuFieldRow = SudokuFieldWindokuRegionRow
                                            * SudokuBoardBlockHeightPlusOne + Iterator3 + 1;
                                        int SudokuFieldSudokuFieldColumn = SudokuFieldWindokuRegionColumn
                                            * SudokuBoardBlockWidthPlusOne + Iterator4 + 1;

                                        if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                            != SudokuFieldSudokuFieldColumn) {
                                            RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                                [SudokuFieldSudokuFieldColumn].remove(
                                                UniqueRemainingNumberInSudokuField);
                                        }
                                    }
                                }
                            }

                            break;
                        case "Center Dot":
                            int BlockCenterRow = SudokuBoardBlockHeight / 2;
                            int BlockCenterColumn = SudokuBoardBlockWidth / 2;

                            if (Iterator1 % SudokuBoardBlockHeight == BlockCenterRow
                                && Iterator2 % SudokuBoardBlockWidth == BlockCenterColumn) {
                                for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                                    for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                                        int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight
                                            + BlockCenterRow;
                                        int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth
                                            + BlockCenterColumn;

                                        if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                            != SudokuFieldSudokuFieldColumn) {
                                            RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                                [SudokuFieldSudokuFieldColumn].remove(
                                                UniqueRemainingNumberInSudokuField);
                                        }
                                    }
                                }
                            }

                            break;
                    }
                }
            }
        }
    }

    /**
     * Gets sudoku fields with unique remaining numbers in relevant columns (sudoku fields with number not present in
     * other sudoku fields in the same column).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns array of sets containing unique remaining numbers in columns for relevant sudoku fields.
     */

    private HashSet<Integer>[][] GetSudokuFieldsWithUniqueRemainingNumberInColumns(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInColumns = new HashSet
            [RemainingNumbersForSudokuFields.length][RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < SudokuFieldsWithUniqueRemainingNumberInColumns.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuFieldsWithUniqueRemainingNumberInColumns.length; Iterator2++) {
                SudokuFieldsWithUniqueRemainingNumberInColumns[Iterator1][Iterator2] = new HashSet<>(
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]);
            }
        }

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length; Iterator3++) {
                    if (Iterator1 != Iterator3) {
                        SudokuFieldsWithUniqueRemainingNumberInColumns[Iterator1][Iterator2].removeAll(
                            RemainingNumbersForSudokuFields[Iterator3][Iterator2]);
                    }
                }
            }
        }

        return SudokuFieldsWithUniqueRemainingNumberInColumns;
    }

    /**
     * Updates remaining numbers for sudoku fields with unique remaining numbers in relevant columns (set for this
     * sudoku field should contain only that unique number and this number should be removed from sets for sudoku fields
     * in the same row, in the same block, etc.).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuFieldsWithUniqueRemainingNumberInColumns Parameter is array of sets containing unique remaining
     * numbers in columns for relevant sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     */

    private void UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInColumns(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInColumns, String SudokuType) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                if (SudokuFieldsWithUniqueRemainingNumberInColumns[Iterator1][Iterator2].size() == 1) {
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]
                        = SudokuFieldsWithUniqueRemainingNumberInColumns[Iterator1][Iterator2];
                    int UniqueRemainingNumberInSudokuField = (int) SudokuFieldsWithUniqueRemainingNumberInColumns
                        [Iterator1][Iterator2].toArray()[0];

                    for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length; Iterator3++) {
                        if (Iterator2 != Iterator3) {
                            RemainingNumbersForSudokuFields[Iterator1][Iterator3].remove(
                                UniqueRemainingNumberInSudokuField);
                        }
                    }

                    int SudokuFieldSudokuFieldBlockRow = Iterator1 / SudokuBoardBlockHeight;
                    int SudokuFieldSudokuFieldBlockColumn = Iterator2 / SudokuBoardBlockWidth;

                    for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                        for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                            int SudokuFieldSudokuFieldRow = SudokuFieldSudokuFieldBlockRow * SudokuBoardBlockHeight
                                + Iterator3;
                            int SudokuFieldSudokuFieldColumn = SudokuFieldSudokuFieldBlockColumn * SudokuBoardBlockWidth
                                + Iterator4;

                            if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2 != SudokuFieldSudokuFieldColumn) {
                                RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                    .remove(UniqueRemainingNumberInSudokuField);
                            }
                        }
                    }

                    switch (SudokuType) {
                        case "Anti-Knight":
                            for (int Iterator3 = 0; Iterator3 < KnightMoves.length; Iterator3++) {
                                int SudokuFieldSudokuFieldRow = Iterator1 + KnightMoves[Iterator3][0];
                                int SudokuFieldSudokuFieldColumn = Iterator2 + KnightMoves[Iterator3][1];

                                if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow
                                    < RemainingNumbersForSudokuFields.length && SudokuFieldSudokuFieldColumn >= 0
                                    && SudokuFieldSudokuFieldColumn < RemainingNumbersForSudokuFields.length) {
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(UniqueRemainingNumberInSudokuField);
                                }
                            }

                            break;
                        case "Diagonal":
                            if (Iterator1 == Iterator2) {
                                for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length;
                                    Iterator3++) {
                                    if (Iterator1 != Iterator3) {
                                        RemainingNumbersForSudokuFields[Iterator3][Iterator3].remove(
                                            UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            if (Iterator1 + Iterator2 == RemainingNumbersForSudokuFields.length - 1) {
                                for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length;
                                    Iterator3++) {
                                    if (Iterator1 != Iterator3) {
                                        RemainingNumbersForSudokuFields[Iterator3]
                                            [RemainingNumbersForSudokuFields.length - Iterator3 - 1].remove(
                                            UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            break;
                        case "Non-Consecutive":
                            for (int Iterator3 = 0; Iterator3 < DirectionsOfNeighbors.length; Iterator3++) {
                                int SudokuFieldSudokuFieldRow = Iterator1 + DirectionsOfNeighbors[Iterator3][0];
                                int SudokuFieldSudokuFieldColumn = Iterator2 + DirectionsOfNeighbors[Iterator3][1];

                                if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow
                                    < RemainingNumbersForSudokuFields.length && SudokuFieldSudokuFieldColumn >= 0
                                    && SudokuFieldSudokuFieldColumn < RemainingNumbersForSudokuFields.length) {
                                    int UniqueRemainingNumberInSudokuFieldMinusOne
                                        = UniqueRemainingNumberInSudokuField - 1;
                                    int UniqueRemainingNumberInSudokuFieldPlusOne
                                        = UniqueRemainingNumberInSudokuField + 1;
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(
                                        UniqueRemainingNumberInSudokuFieldMinusOne);
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(
                                        UniqueRemainingNumberInSudokuFieldPlusOne);
                                }
                            }

                            break;
                        case "Offset":
                            int SudokuFieldSudokuFieldRowInBlock = Iterator1 % SudokuBoardBlockHeight;
                            int SudokuFieldSudokuFieldColumnInBlock = Iterator2 % SudokuBoardBlockWidth;

                            for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                                for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                                    int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight
                                        + SudokuFieldSudokuFieldRowInBlock;
                                    int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth
                                        + SudokuFieldSudokuFieldColumnInBlock;

                                    if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                        != SudokuFieldSudokuFieldColumn) {
                                        RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                            [SudokuFieldSudokuFieldColumn].remove(UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            break;
                        case "Windoku":
                            int SudokuBoardBlockHeightPlusOne = SudokuBoardBlockHeight + 1;
                            int SudokuBoardBlockWidthPlusOne = SudokuBoardBlockWidth + 1;

                            if (Iterator1 % SudokuBoardBlockHeightPlusOne != 0
                                && Iterator2 % SudokuBoardBlockWidthPlusOne != 0) {
                                int SudokuFieldWindokuRegionRow = Iterator1 / SudokuBoardBlockHeightPlusOne;
                                int SudokuFieldWindokuRegionColumn = Iterator2 / SudokuBoardBlockWidthPlusOne;

                                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                                        int SudokuFieldSudokuFieldRow = SudokuFieldWindokuRegionRow
                                            * SudokuBoardBlockHeightPlusOne + Iterator3 + 1;
                                        int SudokuFieldSudokuFieldColumn = SudokuFieldWindokuRegionColumn
                                            * SudokuBoardBlockWidthPlusOne + Iterator4 + 1;

                                        if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                            != SudokuFieldSudokuFieldColumn) {
                                            RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                                [SudokuFieldSudokuFieldColumn].remove(
                                                UniqueRemainingNumberInSudokuField);
                                        }
                                    }
                                }
                            }

                            break;
                        case "Center Dot":
                            int BlockCenterRow = SudokuBoardBlockHeight / 2;
                            int BlockCenterColumn = SudokuBoardBlockWidth / 2;

                            if (Iterator1 % SudokuBoardBlockHeight == BlockCenterRow
                                && Iterator2 % SudokuBoardBlockWidth == BlockCenterColumn) {
                                for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                                    for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                                        int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight
                                            + BlockCenterRow;
                                        int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth
                                            + BlockCenterColumn;

                                        if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                            != SudokuFieldSudokuFieldColumn) {
                                            RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                                [SudokuFieldSudokuFieldColumn].remove(
                                                UniqueRemainingNumberInSudokuField);
                                        }
                                    }
                                }
                            }

                            break;
                    }
                }
            }
        }
    }

    /**
     * Gets sudoku fields with unique remaining numbers in relevant blocks (sudoku fields with number not present in
     * other sudoku fields in the same block).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns array of sets containing unique remaining numbers in blocks for relevant sudoku fields.
     */

    private HashSet<Integer>[][] GetSudokuFieldsWithUniqueRemainingNumberInBlocks(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInBlocks = new HashSet
            [RemainingNumbersForSudokuFields.length][RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < SudokuFieldsWithUniqueRemainingNumberInBlocks.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuFieldsWithUniqueRemainingNumberInBlocks.length; Iterator2++) {
                SudokuFieldsWithUniqueRemainingNumberInBlocks[Iterator1][Iterator2] = new HashSet<>(
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]);
            }
        }

        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                int SudokuFieldSudokuFieldBlockRow = Iterator1 / SudokuBoardBlockHeight;
                int SudokuFieldSudokuFieldBlockColumn = Iterator2 / SudokuBoardBlockWidth;

                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                        int SudokuFieldSudokuFieldRow = SudokuFieldSudokuFieldBlockRow * SudokuBoardBlockHeight
                            + Iterator3;
                        int SudokuFieldSudokuFieldColumn = SudokuFieldSudokuFieldBlockColumn * SudokuBoardBlockWidth
                            + Iterator4;

                        if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2 != SudokuFieldSudokuFieldColumn) {
                            SudokuFieldsWithUniqueRemainingNumberInBlocks[Iterator1][Iterator2].removeAll(
                                RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                [SudokuFieldSudokuFieldColumn]);
                        }
                    }
                }
            }
        }

        return SudokuFieldsWithUniqueRemainingNumberInBlocks;
    }

    /**
     * Updates remaining numbers for sudoku fields with unique remaining numbers in relevant blocks (set for this
     * sudoku field should contain only that unique number and this number should be removed from sets for sudoku fields
     * in the same row, in the same column, etc.).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuFieldsWithUniqueRemainingNumberInBlocks Parameter is array of sets containing unique remaining
     * numbers in blocks for relevant sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     */

    private void UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInBlocks(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInBlocks, String SudokuType) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                if (SudokuFieldsWithUniqueRemainingNumberInBlocks[Iterator1][Iterator2].size() == 1) {
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]
                        = SudokuFieldsWithUniqueRemainingNumberInBlocks[Iterator1][Iterator2];
                    int UniqueRemainingNumberInSudokuField = (int) SudokuFieldsWithUniqueRemainingNumberInBlocks
                        [Iterator1][Iterator2].toArray()[0];

                    for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length; Iterator3++) {
                        if (Iterator2 != Iterator3) {
                            RemainingNumbersForSudokuFields[Iterator1][Iterator3].remove(
                                UniqueRemainingNumberInSudokuField);
                        }
                    }

                    for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length; Iterator3++) {
                        if (Iterator1 != Iterator3) {
                            RemainingNumbersForSudokuFields[Iterator3][Iterator2].remove(
                                UniqueRemainingNumberInSudokuField);
                        }
                    }

                    switch (SudokuType) {
                        case "Anti-Knight":
                            for (int Iterator3 = 0; Iterator3 < KnightMoves.length; Iterator3++) {
                                int SudokuFieldSudokuFieldRow = Iterator1 + KnightMoves[Iterator3][0];
                                int SudokuFieldSudokuFieldColumn = Iterator2 + KnightMoves[Iterator3][1];

                                if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow
                                    < RemainingNumbersForSudokuFields.length && SudokuFieldSudokuFieldColumn >= 0
                                    && SudokuFieldSudokuFieldColumn < RemainingNumbersForSudokuFields.length) {
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(UniqueRemainingNumberInSudokuField);
                                }
                            }

                            break;
                        case "Diagonal":
                            if (Iterator1 == Iterator2) {
                                for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length;
                                    Iterator3++) {
                                    if (Iterator1 != Iterator3) {
                                        RemainingNumbersForSudokuFields[Iterator3][Iterator3].remove(
                                            UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            if (Iterator1 + Iterator2 == RemainingNumbersForSudokuFields.length - 1) {
                                for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length;
                                    Iterator3++) {
                                    if (Iterator1 != Iterator3) {
                                        RemainingNumbersForSudokuFields[Iterator3]
                                            [RemainingNumbersForSudokuFields.length - Iterator3 - 1].remove(
                                            UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            break;
                        case "Non-Consecutive":
                            for (int Iterator3 = 0; Iterator3 < DirectionsOfNeighbors.length; Iterator3++) {
                                int SudokuFieldSudokuFieldRow = Iterator1 + DirectionsOfNeighbors[Iterator3][0];
                                int SudokuFieldSudokuFieldColumn = Iterator2 + DirectionsOfNeighbors[Iterator3][1];

                                if (SudokuFieldSudokuFieldRow >= 0 && SudokuFieldSudokuFieldRow
                                    < RemainingNumbersForSudokuFields.length && SudokuFieldSudokuFieldColumn >= 0
                                    && SudokuFieldSudokuFieldColumn < RemainingNumbersForSudokuFields.length) {
                                    int UniqueRemainingNumberInSudokuFieldMinusOne
                                        = UniqueRemainingNumberInSudokuField - 1;
                                    int UniqueRemainingNumberInSudokuFieldPlusOne
                                        = UniqueRemainingNumberInSudokuField + 1;
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(
                                        UniqueRemainingNumberInSudokuFieldMinusOne);
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                        [SudokuFieldSudokuFieldColumn].remove(
                                        UniqueRemainingNumberInSudokuFieldPlusOne);
                                }
                            }

                            break;
                        case "Offset":
                            int SudokuFieldSudokuFieldRowInBlock = Iterator1 % SudokuBoardBlockHeight;
                            int SudokuFieldSudokuFieldColumnInBlock = Iterator2 % SudokuBoardBlockWidth;

                            for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                                for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                                    int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight
                                        + SudokuFieldSudokuFieldRowInBlock;
                                    int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth
                                        + SudokuFieldSudokuFieldColumnInBlock;

                                    if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                        != SudokuFieldSudokuFieldColumn) {
                                        RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                            [SudokuFieldSudokuFieldColumn].remove(UniqueRemainingNumberInSudokuField);
                                    }
                                }
                            }

                            break;
                        case "Windoku":
                            int SudokuBoardBlockHeightPlusOne = SudokuBoardBlockHeight + 1;
                            int SudokuBoardBlockWidthPlusOne = SudokuBoardBlockWidth + 1;

                            if (Iterator1 % SudokuBoardBlockHeightPlusOne != 0
                                && Iterator2 % SudokuBoardBlockWidthPlusOne != 0) {
                                int SudokuFieldWindokuRegionRow = Iterator1 / SudokuBoardBlockHeightPlusOne;
                                int SudokuFieldWindokuRegionColumn = Iterator2 / SudokuBoardBlockWidthPlusOne;

                                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                                        int SudokuFieldSudokuFieldRow = SudokuFieldWindokuRegionRow
                                            * SudokuBoardBlockHeightPlusOne + Iterator3 + 1;
                                        int SudokuFieldSudokuFieldColumn = SudokuFieldWindokuRegionColumn
                                            * SudokuBoardBlockWidthPlusOne + Iterator4 + 1;

                                        if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                            != SudokuFieldSudokuFieldColumn) {
                                            RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                                [SudokuFieldSudokuFieldColumn].remove(
                                                UniqueRemainingNumberInSudokuField);
                                        }
                                    }
                                }
                            }

                            break;
                        case "Center Dot":
                            int BlockCenterRow = SudokuBoardBlockHeight / 2;
                            int BlockCenterColumn = SudokuBoardBlockWidth / 2;

                            if (Iterator1 % SudokuBoardBlockHeight == BlockCenterRow
                                && Iterator2 % SudokuBoardBlockWidth == BlockCenterColumn) {
                                for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                                    for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                                        int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight
                                            + BlockCenterRow;
                                        int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth
                                            + BlockCenterColumn;

                                        if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2
                                            != SudokuFieldSudokuFieldColumn) {
                                            RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                                [SudokuFieldSudokuFieldColumn].remove(
                                                UniqueRemainingNumberInSudokuField);
                                        }
                                    }
                                }
                            }

                            break;
                    }
                }
            }
        }
    }

    /**
     * Gets sudoku fields with unique remaining numbers in relevant diagonals (sudoku fields with number not present in
     * other sudoku fields in the same diagonal).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns array of sets containing unique remaining numbers in diagonals for relevant sudoku fields.
     */

    private HashSet<Integer>[][] GetSudokuFieldsWithUniqueRemainingNumberInDiagonals(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInDiagonals = new HashSet
            [RemainingNumbersForSudokuFields.length][RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < SudokuFieldsWithUniqueRemainingNumberInDiagonals.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuFieldsWithUniqueRemainingNumberInDiagonals.length; Iterator2++) {
                SudokuFieldsWithUniqueRemainingNumberInDiagonals[Iterator1][Iterator2] = new HashSet<>(
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]);
            }
        }

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                if (Iterator1 != Iterator2) {
                    SudokuFieldsWithUniqueRemainingNumberInDiagonals[Iterator1][Iterator1].removeAll(
                        RemainingNumbersForSudokuFields[Iterator2][Iterator2]);
                }
            }
        }

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                if (Iterator1 != Iterator2) {
                    SudokuFieldsWithUniqueRemainingNumberInDiagonals[Iterator1]
                        [RemainingNumbersForSudokuFields.length - Iterator1 - 1].removeAll(
                        RemainingNumbersForSudokuFields[Iterator2]
                        [RemainingNumbersForSudokuFields.length - Iterator2 - 1]);
                    }
            }
        }

        return SudokuFieldsWithUniqueRemainingNumberInDiagonals;
    }

    /**
     * Updates remaining numbers for sudoku fields with unique remaining numbers in relevant diagonals or in relevant
     * offset regions or in relevant windoku regions or in center dot region (set for this sudoku field should contain
     * only that unique number and this number should be removed from sets for sudoku fields in the same row, in the
     * same column and in the same block).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion
     * Parameter is array of sets containing unique remaining numbers in diagonals or in offset regions or in windoku
     * regions or in center dot region for relevant sudoku fields.
     */

    private void
        UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        HashSet<Integer>[][]
            SudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                if (SudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion
                    [Iterator1][Iterator2].size() == 1) {
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]
                        = SudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion
                        [Iterator1][Iterator2];
                    int UniqueRemainingNumberInSudokuField = (int)
                        SudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion
                        [Iterator1][Iterator2].toArray()[0];

                    for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length; Iterator3++) {
                        if (Iterator2 != Iterator3) {
                            RemainingNumbersForSudokuFields[Iterator1][Iterator3].remove(
                                UniqueRemainingNumberInSudokuField);
                        }
                    }

                    for (int Iterator3 = 0; Iterator3 < RemainingNumbersForSudokuFields.length; Iterator3++) {
                        if (Iterator1 != Iterator3) {
                            RemainingNumbersForSudokuFields[Iterator3][Iterator2].remove(
                                UniqueRemainingNumberInSudokuField);
                        }
                    }

                    int SudokuFieldSudokuFieldBlockRow = Iterator1 / SudokuBoardBlockHeight;
                    int SudokuFieldSudokuFieldBlockColumn = Iterator2 / SudokuBoardBlockWidth;

                    for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                        for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                            int SudokuFieldSudokuFieldRow = SudokuFieldSudokuFieldBlockRow * SudokuBoardBlockHeight
                                + Iterator3;
                            int SudokuFieldSudokuFieldColumn = SudokuFieldSudokuFieldBlockColumn * SudokuBoardBlockWidth
                                + Iterator4;

                            if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2 != SudokuFieldSudokuFieldColumn) {
                                RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                    .remove(UniqueRemainingNumberInSudokuField);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Gets sudoku fields with unique remaining numbers in relevant offset regions (sudoku fields with number not
     * present in other sudoku fields in the same offset region).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns array of sets containing unique remaining numbers in offset regions for relevant sudoku fields.
     */

    private HashSet<Integer>[][] GetSudokuFieldsWithUniqueRemainingNumberInOffsetRegions(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInOffsetRegions = new HashSet
            [RemainingNumbersForSudokuFields.length][RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < SudokuFieldsWithUniqueRemainingNumberInOffsetRegions.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuFieldsWithUniqueRemainingNumberInOffsetRegions.length;
                Iterator2++) {
                SudokuFieldsWithUniqueRemainingNumberInOffsetRegions[Iterator1][Iterator2] = new HashSet<>(
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]);
            }
        }

        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                int SudokuFieldSudokuFieldRowInBlock = Iterator1 % SudokuBoardBlockHeight;
                int SudokuFieldSudokuFieldColumnInBlock = Iterator2 % SudokuBoardBlockWidth;

                for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                        int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight
                            + SudokuFieldSudokuFieldRowInBlock;
                        int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth
                            + SudokuFieldSudokuFieldColumnInBlock;

                        if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2 != SudokuFieldSudokuFieldColumn) {
                            SudokuFieldsWithUniqueRemainingNumberInOffsetRegions[Iterator1][Iterator2].removeAll(
                                RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                [SudokuFieldSudokuFieldColumn]);
                        }
                    }
                }
            }
        }

        return SudokuFieldsWithUniqueRemainingNumberInOffsetRegions;
    }

    /**
     * Gets sudoku fields with unique remaining numbers in relevant windoku regions (sudoku fields with number not
     * present in other sudoku fields in the same windoku region).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns array of sets containing unique remaining numbers in windoku regions for relevant sudoku fields.
     */

    private HashSet<Integer>[][] GetSudokuFieldsWithUniqueRemainingNumberInWindokuRegions(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInWindokuRegions = new HashSet
            [RemainingNumbersForSudokuFields.length][RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < SudokuFieldsWithUniqueRemainingNumberInWindokuRegions.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuFieldsWithUniqueRemainingNumberInWindokuRegions.length;
                Iterator2++) {
                SudokuFieldsWithUniqueRemainingNumberInWindokuRegions[Iterator1][Iterator2] = new HashSet<>(
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]);
            }
        }

        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int SudokuBoardBlockHeightPlusOne = SudokuBoardBlockHeight + 1;
        int SudokuBoardBlockWidthPlusOne = SudokuBoardBlockWidth + 1;

        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                if (Iterator1 % SudokuBoardBlockHeightPlusOne != 0 && Iterator2 % SudokuBoardBlockWidthPlusOne != 0) {
                    int SudokuFieldWindokuRegionRow = Iterator1 / SudokuBoardBlockHeightPlusOne;
                    int SudokuFieldWindokuRegionColumn = Iterator2 / SudokuBoardBlockWidthPlusOne;

                    for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                        for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                            int SudokuFieldSudokuFieldRow = SudokuFieldWindokuRegionRow * SudokuBoardBlockHeightPlusOne
                                + Iterator3 + 1;
                            int SudokuFieldSudokuFieldColumn = SudokuFieldWindokuRegionColumn
                                * SudokuBoardBlockWidthPlusOne + Iterator4 + 1;

                            if (Iterator1 != SudokuFieldSudokuFieldRow || Iterator2 != SudokuFieldSudokuFieldColumn) {
                                SudokuFieldsWithUniqueRemainingNumberInWindokuRegions[Iterator1][Iterator2].removeAll(
                                    RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow]
                                    [SudokuFieldSudokuFieldColumn]);
                            }
                        }
                    }
                }
            }
        }

        return SudokuFieldsWithUniqueRemainingNumberInWindokuRegions;
    }

    /**
     * Gets sudoku fields with unique remaining numbers in center dot region (sudoku fields with number not present in
     * other sudoku fields in center dot region).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @return Returns array of sets containing unique remaining numbers in center dot region for relevant sudoku
     * fields.
     */

    private HashSet<Integer>[][] GetSudokuFieldsWithUniqueRemainingNumberInCenterDotRegion(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields) {
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInCenterDotRegion = new HashSet
            [RemainingNumbersForSudokuFields.length][RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < SudokuFieldsWithUniqueRemainingNumberInCenterDotRegion.length;
            Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuFieldsWithUniqueRemainingNumberInCenterDotRegion.length;
                Iterator2++) {
                SudokuFieldsWithUniqueRemainingNumberInCenterDotRegion[Iterator1][Iterator2] = new HashSet<>(
                    RemainingNumbersForSudokuFields[Iterator1][Iterator2]);
            }
        }

        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;
        int BlockCenterRow = SudokuBoardBlockHeight / 2;
        int BlockCenterColumn = SudokuBoardBlockWidth / 2;

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                        int SudokuFieldSudokuFieldRow1 = Iterator1 * SudokuBoardBlockHeight + BlockCenterRow;
                        int SudokuFieldSudokuFieldColumn1 = Iterator2 * SudokuBoardBlockWidth + BlockCenterColumn;
                        int SudokuFieldSudokuFieldRow2 = Iterator3 * SudokuBoardBlockHeight + BlockCenterRow;
                        int SudokuFieldSudokuFieldColumn2 = Iterator4 * SudokuBoardBlockWidth + BlockCenterColumn;

                        if (SudokuFieldSudokuFieldRow1 != SudokuFieldSudokuFieldRow2 || SudokuFieldSudokuFieldColumn1
                            != SudokuFieldSudokuFieldColumn2) {
                            SudokuFieldsWithUniqueRemainingNumberInCenterDotRegion[SudokuFieldSudokuFieldRow1]
                                [SudokuFieldSudokuFieldColumn1].removeAll(RemainingNumbersForSudokuFields
                                [SudokuFieldSudokuFieldRow2][SudokuFieldSudokuFieldColumn2]);
                        }
                    }
                }
            }
        }

        return SudokuFieldsWithUniqueRemainingNumberInCenterDotRegion;
    }

    /**
     * Gets sudoku fields with unique remaining numbers in relevant rows, in relevant columns, in relevant blocks, etc.
     * and updates remaining numbers for sudoku fields (all possible numbers to fill sudoku fields).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns array of sets containing deleted numbers for relevant sudoku fields (needed for backtracking).
     */

    private HashSet<Integer>[][] GetSudokuFieldsWithUniqueRemainingNumberAndUpdateRemainingNumbersForSudokuFields(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, String SudokuType) {
        HashSet<Integer>[][] DeletedNumbersForSudokuFields = new HashSet[RemainingNumbersForSudokuFields.length]
            [RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < DeletedNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < DeletedNumbersForSudokuFields.length; Iterator2++) {
                DeletedNumbersForSudokuFields[Iterator1][Iterator2] = new HashSet<>(RemainingNumbersForSudokuFields
                    [Iterator1][Iterator2]);
            }
        }

        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInRows
            = GetSudokuFieldsWithUniqueRemainingNumberInRows(RemainingNumbersForSudokuFields);
        UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInRows(RemainingNumbersForSudokuFields,
            SudokuFieldsWithUniqueRemainingNumberInRows, SudokuType);
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInColumns
            = GetSudokuFieldsWithUniqueRemainingNumberInColumns(RemainingNumbersForSudokuFields);
        UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInColumns(RemainingNumbersForSudokuFields,
            SudokuFieldsWithUniqueRemainingNumberInColumns, SudokuType);
        HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInBlocks
            = GetSudokuFieldsWithUniqueRemainingNumberInBlocks(RemainingNumbersForSudokuFields);
        UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInBlocks(RemainingNumbersForSudokuFields,
            SudokuFieldsWithUniqueRemainingNumberInBlocks, SudokuType);

        switch (SudokuType) {
            case "Diagonal":
                HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInDiagonals
                    = GetSudokuFieldsWithUniqueRemainingNumberInDiagonals(RemainingNumbersForSudokuFields);
                UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion(
                    RemainingNumbersForSudokuFields, SudokuFieldsWithUniqueRemainingNumberInDiagonals);
                break;
            case "Offset":
                HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInOffsetRegions
                    = GetSudokuFieldsWithUniqueRemainingNumberInOffsetRegions(RemainingNumbersForSudokuFields);
                UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion(
                    RemainingNumbersForSudokuFields, SudokuFieldsWithUniqueRemainingNumberInOffsetRegions);
                break;
            case "Windoku":
                HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInWindokuRegions
                    = GetSudokuFieldsWithUniqueRemainingNumberInWindokuRegions(RemainingNumbersForSudokuFields);
                UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion(
                    RemainingNumbersForSudokuFields, SudokuFieldsWithUniqueRemainingNumberInWindokuRegions);
                break;
            case "Center Dot":
                HashSet<Integer>[][] SudokuFieldsWithUniqueRemainingNumberInCenterDotRegion
                    = GetSudokuFieldsWithUniqueRemainingNumberInCenterDotRegion(RemainingNumbersForSudokuFields);
                UpdateRemainingNumbersForSudokuFieldsWithUniqueRemainingNumberInDiagonalsOrInOffsetRegionsOrInWindokuRegionsOrInCenterDotRegion(
                    RemainingNumbersForSudokuFields, SudokuFieldsWithUniqueRemainingNumberInCenterDotRegion);
                break;
        }

        for (int Iterator1 = 0; Iterator1 < DeletedNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < DeletedNumbersForSudokuFields.length; Iterator2++) {
                DeletedNumbersForSudokuFields[Iterator1][Iterator2].removeAll(RemainingNumbersForSudokuFields[Iterator1]
                    [Iterator2]);
            }
        }

        return DeletedNumbersForSudokuFields;
    }

    /**
     * Gets not filled sudoku fields in relevant rows.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @return Returns array of sets containing not filled sudoku fields in relevant rows.
     */

    private HashSet<SudokuField>[] GetNotFilledSudokuFieldsInRows(int[][] SudokuBoardArray) {
        HashSet<SudokuField>[] NotFilledSudokuFieldsInRows = new HashSet[SudokuBoardArray.length];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            NotFilledSudokuFieldsInRows[Iterator1] = new HashSet<>();

            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                if (SudokuBoardArray[Iterator1][Iterator2] == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NotFilledSudokuFieldsInRows[Iterator1].add(new SudokuField(Iterator1, Iterator2));
                }
            }
        }

        return NotFilledSudokuFieldsInRows;
    }

     /**
     * Gets not filled sudoku fields in relevant columns.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @return Returns array of sets containing not filled sudoku fields in relevant columns.
     */

    private HashSet<SudokuField>[] GetNotFilledSudokuFieldsInColumns(int[][] SudokuBoardArray) {
        HashSet<SudokuField>[] NotFilledSudokuFieldsInColumns = new HashSet[SudokuBoardArray.length];

        for (int Iterator1 = 0; Iterator1 < SudokuBoardArray.length; Iterator1++) {
            NotFilledSudokuFieldsInColumns[Iterator1] = new HashSet<>();

            for (int Iterator2 = 0; Iterator2 < SudokuBoardArray.length; Iterator2++) {
                if (SudokuBoardArray[Iterator2][Iterator1] == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NotFilledSudokuFieldsInColumns[Iterator1].add(new SudokuField(Iterator2, Iterator1));
                }
            }
        }

        return NotFilledSudokuFieldsInColumns;
    }

     /**
     * Gets not filled sudoku fields in relevant blocks.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @return Returns array of sets containing not filled sudoku fields in relevant blocks.
     */

    private HashSet<SudokuField>[] GetNotFilledSudokuFieldsInBlocks(int[][] SudokuBoardArray) {
        HashSet<SudokuField>[] NotFilledSudokuFieldsInBlocks = new HashSet[SudokuBoardArray.length];
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = SudokuBoardArray.length / SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                int SudokuBoardBlockNumber = Iterator1 * BlocksColumns + Iterator2;
                NotFilledSudokuFieldsInBlocks[SudokuBoardBlockNumber] = new HashSet<>();

                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                        int SudokuFieldSudokuFieldRow = Iterator1 * SudokuBoardBlockHeight + Iterator3;
                        int SudokuFieldSudokuFieldColumn = Iterator2 * SudokuBoardBlockWidth + Iterator4;

                        if (SudokuBoardArray[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                            == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NotFilledSudokuFieldsInBlocks[SudokuBoardBlockNumber].add(new SudokuField(
                                SudokuFieldSudokuFieldRow, SudokuFieldSudokuFieldColumn));
                        }
                    }
                }
            }
        }

        return NotFilledSudokuFieldsInBlocks;
    }

    /**
     * Gets not filled sudoku fields in relevant diagonals.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @return Returns array of sets containing not filled sudoku fields in relevant diagonals.
     */

    private HashSet<SudokuField>[] GetNotFilledSudokuFieldsInDiagonals(int[][] SudokuBoardArray) {
        HashSet<SudokuField>[] NotFilledSudokuFieldsInDiagonals = new HashSet[2];
        HashSet<SudokuField> NotFilledSudokuFieldsInDiagonal1 = new HashSet<>();

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            if (SudokuBoardArray[Iterator][Iterator] == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                NotFilledSudokuFieldsInDiagonal1.add(new SudokuField(Iterator, Iterator));
            }
        }

        NotFilledSudokuFieldsInDiagonals[0] = NotFilledSudokuFieldsInDiagonal1;
        HashSet<SudokuField> NotFilledSudokuFieldsInDiagonal2 = new HashSet<>();

        for (int Iterator = 0; Iterator < SudokuBoardArray.length; Iterator++) {
            if (SudokuBoardArray[Iterator][SudokuBoardArray.length - Iterator - 1]
                == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                NotFilledSudokuFieldsInDiagonal2.add(new SudokuField(Iterator,
                SudokuBoardArray.length - Iterator - 1));
            }
        }

        NotFilledSudokuFieldsInDiagonals[1] = NotFilledSudokuFieldsInDiagonal2;
        return NotFilledSudokuFieldsInDiagonals;
    }

    /**
     * Gets not filled sudoku fields in relevant offset regions.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @return Returns array of sets containing not filled sudoku fields in relevant offset regions.
     */

    private HashSet<SudokuField>[] GetNotFilledSudokuFieldsInOffsetRegions(int[][] SudokuBoardArray) {
        HashSet<SudokuField>[] NotFilledSudokuFieldsInOffsetRegions = new HashSet[SudokuBoardArray.length];
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = SudokuBoardArray.length / SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                int SudokuBoardOffsetRegionNumber = Iterator1 * SudokuBoardBlockWidth + Iterator2;
                NotFilledSudokuFieldsInOffsetRegions[SudokuBoardOffsetRegionNumber] = new HashSet<>();

                for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                        int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight + Iterator1;
                        int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth + Iterator2;

                        if (SudokuBoardArray[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                            == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NotFilledSudokuFieldsInOffsetRegions[SudokuBoardOffsetRegionNumber].add(new SudokuField(
                                SudokuFieldSudokuFieldRow, SudokuFieldSudokuFieldColumn));
                        }
                    }
                }
            }
        }

        return NotFilledSudokuFieldsInOffsetRegions;
    }

    /**
     * Gets not filled sudoku fields in relevant windoku regions.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @return Returns array of sets containing not filled sudoku fields in relevant windoku regions.
     */

    private HashSet<SudokuField>[] GetNotFilledSudokuFieldsInWindokuRegions(int[][] SudokuBoardArray) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = SudokuBoardArray.length / SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardBlockWidth;
        int BlocksRowsMinusOne = BlocksRows - 1;
        int BlocksColumnsMinusOne = BlocksColumns - 1;
        HashSet<SudokuField>[] NotFilledSudokuFieldsInWindokuRegions = new HashSet[BlocksRowsMinusOne
            * BlocksColumnsMinusOne];

        for (int Iterator1 = 0; Iterator1 < BlocksRowsMinusOne; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumnsMinusOne; Iterator2++) {
                int SudokuBoardWindokuRegionNumber = Iterator1 * BlocksColumnsMinusOne + Iterator2;
                NotFilledSudokuFieldsInWindokuRegions[SudokuBoardWindokuRegionNumber] = new HashSet<>();

                for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                    for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                        int SudokuFieldSudokuFieldRow = Iterator1 * (SudokuBoardBlockHeight + 1) + Iterator3 + 1;
                        int SudokuFieldSudokuFieldColumn = Iterator2 * (SudokuBoardBlockWidth + 1) + Iterator4 + 1;

                        if (SudokuBoardArray[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                            == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                            NotFilledSudokuFieldsInWindokuRegions[SudokuBoardWindokuRegionNumber].add(new SudokuField(
                                SudokuFieldSudokuFieldRow, SudokuFieldSudokuFieldColumn));
                        }
                    }
                }
            }
        }

        return NotFilledSudokuFieldsInWindokuRegions;
    }

    /**
     * Gets not filled sudoku fields in center dot region.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @return Returns array of sets containing not filled sudoku fields in center dot region.
     */

    private HashSet<SudokuField>[] GetNotFilledSudokuFieldsInCenterDotRegion(int[][] SudokuBoardArray) {
        HashSet<SudokuField>[] NotFilledSudokuFieldsInCenterDotRegion = new HashSet[1];
        HashSet<SudokuField> NotFilledSudokuFieldsInCenterDotRegion1 = new HashSet<>();
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            SudokuBoardArray.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = SudokuBoardArray.length / SudokuBoardBlockHeight;
        int BlocksColumns = SudokuBoardArray.length / SudokuBoardBlockWidth;
        int BlockCenterRow = SudokuBoardBlockHeight / 2;
        int BlockCenterColumn = SudokuBoardBlockWidth / 2;

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                if (SudokuBoardArray[Iterator1 * SudokuBoardBlockHeight + BlockCenterRow][Iterator2
                    * SudokuBoardBlockWidth + BlockCenterColumn] == DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE) {
                    NotFilledSudokuFieldsInCenterDotRegion1.add(new SudokuField(
                        Iterator1 * SudokuBoardBlockHeight + BlockCenterRow, Iterator2
                        * SudokuBoardBlockWidth + BlockCenterColumn));
                }
            }
        }

        NotFilledSudokuFieldsInCenterDotRegion[0] = NotFilledSudokuFieldsInCenterDotRegion1;
        return NotFilledSudokuFieldsInCenterDotRegion;
    }

    /**
     * Gets factorial of given number.
     * @param Number Parameter is number for which factorial will be calculated.
     * @return Returns factorial of given number.
     */

    private int GetFactorial(int Number) {
        return (Number == 1 || Number == 0) ? 1 : Number * GetFactorial(Number - 1);
    }

    /**
     * Gets number of all possible boolean permutations for given number of not filled sudoku fields and given
     * preemptive set size.
     * @param NumberOfNotFilledSudokuFields Parameter is number of not filled sudoku fields.
     * @param PreemptiveSetSize Parameter is preemptive set size.
     * @return Returns number of all possible boolean permutations for given number of not filled sudoku fields and
     * given preemptive set size.
     */

    private int GetNumberOfPermutations(int NumberOfNotFilledSudokuFields, int PreemptiveSetSize) {
        return GetFactorial(NumberOfNotFilledSudokuFields) / (GetFactorial(PreemptiveSetSize) * GetFactorial(
            NumberOfNotFilledSudokuFields - PreemptiveSetSize));
    }

    /**
     * Gets all possible boolean permutations for given number of not filled sudoku fields and given preemptive set
     * size.
     * @param NumberOfNotFilledSudokuFields Parameter is number of not filled sudoku fields.
     * @param PreemptiveSetSize Parameter is preemptive set size.
     * @return Returns all possible boolean permutations for given number of not filled sudoku fields and given
     * preemptive set size.
     */

    private HashSet<boolean[]> GetAllBooleanPermutations(int NumberOfNotFilledSudokuFields, int PreemptiveSetSize) {
        int NumberOfPermutations = GetNumberOfPermutations(NumberOfNotFilledSudokuFields, PreemptiveSetSize);
        HashSet<boolean[]> BooleanPermutations = new HashSet<>();
        int BitsPermutation = 0;

        for (int Iterator = 0; Iterator < PreemptiveSetSize; Iterator++) {
            BitsPermutation = BitsPermutation << 1;
            BitsPermutation++;
        }

        int NumberOfNotFilledSudokuFieldsMinusPreemptiveSetSize = NumberOfNotFilledSudokuFields - PreemptiveSetSize;

        for (int Iterator = 0; Iterator < NumberOfNotFilledSudokuFieldsMinusPreemptiveSetSize; Iterator++) {
            BitsPermutation = BitsPermutation << 1;
        }

        BitsPermutation = BitsPermutation >>> NumberOfNotFilledSudokuFieldsMinusPreemptiveSetSize;

        for (int Iterator = 0; Iterator < NumberOfPermutations; Iterator++) {
            boolean[] BooleanPermutation = new boolean[NumberOfNotFilledSudokuFields];
            int CurrentBitsPermutation = BitsPermutation;

            for (int Iterator2 = NumberOfNotFilledSudokuFields - 1; Iterator2 >= 0; Iterator2--) {
                BooleanPermutation[Iterator2] = ((CurrentBitsPermutation & 1) == 1);
                CurrentBitsPermutation = CurrentBitsPermutation >> 1;
            }

            BooleanPermutations.add(BooleanPermutation);
            int BitsPermutationMove = (BitsPermutation | (BitsPermutation - 1)) + 1;
            BitsPermutation = BitsPermutationMove | ((((BitsPermutationMove & -BitsPermutationMove) / (BitsPermutation
                & -BitsPermutation)) >> 1) - 1);
        }

        return BooleanPermutations;
    }

    /**
     * Gets all preemptive sets for given not filled sudoku fields and given preemptive set size.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param NotFilledSudokuFields Parameter is array of sets containing not filled sudoku fields.
     * @param PreemptiveSetSize Parameter is preemptive set size.
     * @return Returns all preemptive sets for given not filled sudoku fields and given preemptive set size.
     */

    private PreemptiveSet[] GetPreemptiveSets(HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        HashSet<SudokuField>[] NotFilledSudokuFields, int PreemptiveSetSize) {
        PreemptiveSet[] PreemptiveSets = new PreemptiveSet[NotFilledSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < NotFilledSudokuFields.length; Iterator1++) {
            PreemptiveSets[Iterator1] = new PreemptiveSet(new HashSet<>(), new HashSet<>());

            if (NotFilledSudokuFields[Iterator1].size() >= PreemptiveSetSize) {
                HashSet<boolean[]> BooleanPermutations = GetAllBooleanPermutations(
                    NotFilledSudokuFields[Iterator1].size(), PreemptiveSetSize);

                for (int Iterator2 = 0; Iterator2 < BooleanPermutations.size(); Iterator2++) {
                    HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSet = new HashSet<>();
                    HashSet<SudokuField> SudokuFieldsFormingPreemptiveSet = new HashSet<>();
                    boolean[] BooleanPermutation = (boolean[]) BooleanPermutations.toArray()[Iterator2];

                    for (int Iterator3 = 0; Iterator3 < BooleanPermutation.length; Iterator3++) {
                        if (BooleanPermutation[Iterator3]) {
                            SudokuField NotFilledSudokuField = (SudokuField) NotFilledSudokuFields[Iterator1].toArray()
                                [Iterator3];
                            int NotFilledSudokuFieldSudokuFieldRow = NotFilledSudokuField.SudokuFieldRow;
                            int NotFilledSudokuFieldSudokuFieldColumn = NotFilledSudokuField.SudokuFieldColumn;
                            RemainingNumbersForSudokuFieldsFormingPreemptiveSet.addAll(RemainingNumbersForSudokuFields
                                [NotFilledSudokuFieldSudokuFieldRow][NotFilledSudokuFieldSudokuFieldColumn]);
                            SudokuFieldsFormingPreemptiveSet.add(NotFilledSudokuField);
                        }
                    }

                    if (RemainingNumbersForSudokuFieldsFormingPreemptiveSet.size() == PreemptiveSetSize) {
                        PreemptiveSets[Iterator1].RemainingNumbersForSudokuFieldsFormingPreemptiveSet
                            = RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
                        PreemptiveSets[Iterator1].SudokuFieldsFormingPreemptiveSet = SudokuFieldsFormingPreemptiveSet;
                        break;
                    }
                }
            }
        }

        return PreemptiveSets;
    }

    /**
     * Checks if given set of sudoku fields forming preemptive set contains sudoku field with given row and given
     * column.
     * @param SudokuFieldsFormingPreemptiveSet Parameter is set containing sudoku fields forming preemptive set.
     * @param SudokuFieldSudokuFieldRow Parameter is row of sudoku field.
     * @param SudokuFieldSudokuFieldColumn Parameter is column of sudoku field.
     * @return Returns if given set of sudoku fields forming preemptive set contains sudoku field with given row and
     * given column.
     */

    private boolean CheckIfContainsSudokuField(HashSet<SudokuField> SudokuFieldsFormingPreemptiveSet,
        int SudokuFieldSudokuFieldRow, int SudokuFieldSudokuFieldColumn) {
        for (int Iterator = 0; Iterator < SudokuFieldsFormingPreemptiveSet.size(); Iterator++) {
            SudokuField SudokuFieldFormingPreemptiveSet = (SudokuField) SudokuFieldsFormingPreemptiveSet.toArray()
                [Iterator];

            if (SudokuFieldFormingPreemptiveSet.SudokuFieldRow == SudokuFieldSudokuFieldRow
                && SudokuFieldFormingPreemptiveSet.SudokuFieldColumn == SudokuFieldSudokuFieldColumn) {
                return true;
            }
        }

        return false;
    }

    /**
     * Updates remaining numbers for sudoku fields using preemptive sets in rows.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param PreemptiveSetsInRows Parameter is array of PreemptiveSet objects containing remaining numbers for sudoku
     * fields forming relevant preemptive sets in rows and sudoku fields forming relevant preemptive sets in rows.
     */

    private void UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInRows(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, PreemptiveSet[] PreemptiveSetsInRows) {
        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSetInRow = PreemptiveSetsInRows[Iterator1]
                .RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
            HashSet<SudokuField> SudokuFieldsFormingPreemptiveSetInRow = PreemptiveSetsInRows[Iterator1]
                .SudokuFieldsFormingPreemptiveSet;

            if (RemainingNumbersForSudokuFieldsFormingPreemptiveSetInRow.size() > 0
                && SudokuFieldsFormingPreemptiveSetInRow.size() > 0) {
                for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                    if (!CheckIfContainsSudokuField(SudokuFieldsFormingPreemptiveSetInRow, Iterator1, Iterator2)) {
                        RemainingNumbersForSudokuFields[Iterator1][Iterator2].removeAll(
                            RemainingNumbersForSudokuFieldsFormingPreemptiveSetInRow);
                    }
                }
            }
        }
    }

    /**
     * Updates remaining numbers for sudoku fields using preemptive sets in columns.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param PreemptiveSetsInColumns Parameter is array of PreemptiveSet objects containing remaining numbers for
     * sudoku fields forming relevant preemptive sets in columns and sudoku fields forming relevant preemptive sets in
     * columns.
     */

    private void UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInColumns(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, PreemptiveSet[] PreemptiveSetsInColumns) {
        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSetInColumn
                = PreemptiveSetsInColumns[Iterator1].RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
            HashSet<SudokuField> SudokuFieldsFormingPreemptiveSetInColumn = PreemptiveSetsInColumns[Iterator1]
                .SudokuFieldsFormingPreemptiveSet;

            if (RemainingNumbersForSudokuFieldsFormingPreemptiveSetInColumn.size() > 0
                && SudokuFieldsFormingPreemptiveSetInColumn.size() > 0) {
                for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                    if (!CheckIfContainsSudokuField(SudokuFieldsFormingPreemptiveSetInColumn, Iterator2, Iterator1)) {
                        RemainingNumbersForSudokuFields[Iterator2][Iterator1].removeAll(
                            RemainingNumbersForSudokuFieldsFormingPreemptiveSetInColumn);
                    }
                }
            }
        }
    }

    /**
     * Updates remaining numbers for sudoku fields using preemptive sets in blocks.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param PreemptiveSetsInBlocks Parameter is array of PreemptiveSet objects containing remaining numbers for sudoku
     * fields forming relevant preemptive sets in blocks and sudoku fields forming relevant preemptive sets in blocks.
     */

    private void UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInBlocks(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, PreemptiveSet[] PreemptiveSetsInBlocks) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                int SudokuBoardBlockNumber = Iterator1 * BlocksColumns + Iterator2;
                HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSetInBlock
                    = PreemptiveSetsInBlocks[SudokuBoardBlockNumber]
                    .RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
                HashSet<SudokuField> SudokuFieldsFormingPreemptiveSetInBlock
                    = PreemptiveSetsInBlocks[SudokuBoardBlockNumber].SudokuFieldsFormingPreemptiveSet;

                if (RemainingNumbersForSudokuFieldsFormingPreemptiveSetInBlock.size() > 0
                    && SudokuFieldsFormingPreemptiveSetInBlock.size() > 0) {
                    for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                        for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                            int SudokuFieldSudokuFieldRow = Iterator1 * SudokuBoardBlockHeight + Iterator3;
                            int SudokuFieldSudokuFieldColumn = Iterator2 * SudokuBoardBlockWidth + Iterator4;

                            if (!CheckIfContainsSudokuField(SudokuFieldsFormingPreemptiveSetInBlock,
                                SudokuFieldSudokuFieldRow, SudokuFieldSudokuFieldColumn)) {
                                RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                    .removeAll(RemainingNumbersForSudokuFieldsFormingPreemptiveSetInBlock);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Updates remaining numbers for sudoku fields using preemptive sets in diagonals.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param PreemptiveSetsInDiagonals Parameter is array of PreemptiveSet objects containing remaining numbers for
     * sudoku fields forming relevant preemptive sets in diagonals and sudoku fields forming relevant preemptive sets in
     * diagonals.
     */

    private void UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInDiagonals(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, PreemptiveSet[] PreemptiveSetsInDiagonals) {
        HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSetInDiagonal1 = PreemptiveSetsInDiagonals[0]
            .RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
        HashSet<SudokuField> SudokuFieldsFormingPreemptiveSetInDiagonal1 = PreemptiveSetsInDiagonals[0]
            .SudokuFieldsFormingPreemptiveSet;

        if (RemainingNumbersForSudokuFieldsFormingPreemptiveSetInDiagonal1.size() > 0
            && SudokuFieldsFormingPreemptiveSetInDiagonal1.size() > 0) {
            for (int Iterator = 0; Iterator < RemainingNumbersForSudokuFields.length; Iterator++) {
                if (!CheckIfContainsSudokuField(SudokuFieldsFormingPreemptiveSetInDiagonal1, Iterator, Iterator)) {
                    RemainingNumbersForSudokuFields[Iterator][Iterator].removeAll(
                        RemainingNumbersForSudokuFieldsFormingPreemptiveSetInDiagonal1);
                }
            }
        }

        HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSetInDiagonal2 = PreemptiveSetsInDiagonals[1]
            .RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
        HashSet<SudokuField> SudokuFieldsFormingPreemptiveSetInDiagonal2 = PreemptiveSetsInDiagonals[1]
            .SudokuFieldsFormingPreemptiveSet;

        if (RemainingNumbersForSudokuFieldsFormingPreemptiveSetInDiagonal2.size() > 0
            && SudokuFieldsFormingPreemptiveSetInDiagonal2.size() > 0) {
            for (int Iterator = 0; Iterator < RemainingNumbersForSudokuFields.length; Iterator++) {
                if (!CheckIfContainsSudokuField(SudokuFieldsFormingPreemptiveSetInDiagonal2, Iterator,
                    RemainingNumbersForSudokuFields.length - Iterator - 1)) {
                    RemainingNumbersForSudokuFields[Iterator][RemainingNumbersForSudokuFields.length - Iterator - 1]
                        .removeAll(RemainingNumbersForSudokuFieldsFormingPreemptiveSetInDiagonal2);
                }
            }
        }
    }

    /**
     * Updates remaining numbers for sudoku fields using preemptive sets in offset regions.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param PreemptiveSetsInOffsetRegions Parameter is array of PreemptiveSet objects containing remaining numbers for
     * sudoku fields forming relevant preemptive sets in offset regions and sudoku fields forming relevant preemptive
     * sets in offset regions.
     */

    private void UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInOffsetRegions(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, PreemptiveSet[] PreemptiveSetsInOffsetRegions) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;

        for (int Iterator1 = 0; Iterator1 < SudokuBoardBlockHeight; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < SudokuBoardBlockWidth; Iterator2++) {
                int SudokuBoardOffsetRegionNumber = Iterator1 * SudokuBoardBlockWidth + Iterator2;
                HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSetInOffsetRegion
                    = PreemptiveSetsInOffsetRegions[SudokuBoardOffsetRegionNumber]
                    .RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
                HashSet<SudokuField> SudokuFieldsFormingPreemptiveSetInOffsetRegion
                    = PreemptiveSetsInOffsetRegions[SudokuBoardOffsetRegionNumber].SudokuFieldsFormingPreemptiveSet;

                if (RemainingNumbersForSudokuFieldsFormingPreemptiveSetInOffsetRegion.size() > 0
                    && SudokuFieldsFormingPreemptiveSetInOffsetRegion.size() > 0) {
                    for (int Iterator3 = 0; Iterator3 < BlocksRows; Iterator3++) {
                        for (int Iterator4 = 0; Iterator4 < BlocksColumns; Iterator4++) {
                            int SudokuFieldSudokuFieldRow = Iterator3 * SudokuBoardBlockHeight + Iterator1;
                            int SudokuFieldSudokuFieldColumn = Iterator4 * SudokuBoardBlockWidth + Iterator2;

                            if (!CheckIfContainsSudokuField(SudokuFieldsFormingPreemptiveSetInOffsetRegion,
                                SudokuFieldSudokuFieldRow, SudokuFieldSudokuFieldColumn)) {
                                RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                    .removeAll(RemainingNumbersForSudokuFieldsFormingPreemptiveSetInOffsetRegion);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Updates remaining numbers for sudoku fields using preemptive sets in windoku regions.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param PreemptiveSetsInWindokuRegions Parameter is array of PreemptiveSet objects containing remaining numbers
     * for sudoku fields forming relevant preemptive sets in windoku regions and sudoku fields forming relevant
     * preemptive sets in windoku regions.
     */

    private void UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInWindokuRegions(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, PreemptiveSet[] PreemptiveSetsInWindokuRegions) {
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;
        int BlocksRowsMinusOne = BlocksRows - 1;
        int BlocksColumnsMinusOne = BlocksColumns - 1;

        for (int Iterator1 = 0; Iterator1 < BlocksRowsMinusOne; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < BlocksColumnsMinusOne; Iterator2++) {
                int SudokuBoardWindokuRegionNumber = Iterator1 * BlocksColumnsMinusOne + Iterator2;
                HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSetInWindokuRegion
                    = PreemptiveSetsInWindokuRegions[SudokuBoardWindokuRegionNumber]
                    .RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
                HashSet<SudokuField> SudokuFieldsFormingPreemptiveSetInWindokuRegion
                    = PreemptiveSetsInWindokuRegions[SudokuBoardWindokuRegionNumber].SudokuFieldsFormingPreemptiveSet;

                if (RemainingNumbersForSudokuFieldsFormingPreemptiveSetInWindokuRegion.size() > 0
                    && SudokuFieldsFormingPreemptiveSetInWindokuRegion.size() > 0) {
                    for (int Iterator3 = 0; Iterator3 < SudokuBoardBlockHeight; Iterator3++) {
                        for (int Iterator4 = 0; Iterator4 < SudokuBoardBlockWidth; Iterator4++) {
                            int SudokuFieldSudokuFieldRow = Iterator1 * (SudokuBoardBlockHeight + 1) + Iterator3 + 1;
                            int SudokuFieldSudokuFieldColumn = Iterator2 * (SudokuBoardBlockWidth + 1) + Iterator4 + 1;

                            if (!CheckIfContainsSudokuField(SudokuFieldsFormingPreemptiveSetInWindokuRegion,
                                SudokuFieldSudokuFieldRow, SudokuFieldSudokuFieldColumn)) {
                                RemainingNumbersForSudokuFields[SudokuFieldSudokuFieldRow][SudokuFieldSudokuFieldColumn]
                                    .removeAll(RemainingNumbersForSudokuFieldsFormingPreemptiveSetInWindokuRegion);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Updates remaining numbers for sudoku fields using preemptive sets in center dot region.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param PreemptiveSetsInCenterDotRegion Parameter is array of PreemptiveSet objects containing remaining numbers
     * for sudoku fields forming relevant preemptive sets in center dot region and sudoku fields forming relevant
     * preemptive sets in center dot region.
     */

    private void UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInCenterDotRegion(
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, PreemptiveSet[] PreemptiveSetsInCenterDotRegion) {
        HashSet<Integer> RemainingNumbersForSudokuFieldsFormingPreemptiveSetInCenterDotRegion
            = PreemptiveSetsInCenterDotRegion[0].RemainingNumbersForSudokuFieldsFormingPreemptiveSet;
        HashSet<SudokuField> SudokuFieldsFormingPreemptiveSetInCenterDotRegion = PreemptiveSetsInCenterDotRegion[0]
            .SudokuFieldsFormingPreemptiveSet;
        SudokuBoardSizeAndBlockSize SudokuBoardSizeAndBlockSize = GetSudokuBoardSizeAndBlockSizeBySudokuBoardSize(
            RemainingNumbersForSudokuFields.length);
        int SudokuBoardBlockHeight = SudokuBoardSizeAndBlockSize.SudokuBoardBlockHeight;
        int SudokuBoardBlockWidth = SudokuBoardSizeAndBlockSize.SudokuBoardBlockWidth;
        int BlocksRows = RemainingNumbersForSudokuFields.length / SudokuBoardBlockHeight;
        int BlocksColumns = RemainingNumbersForSudokuFields.length / SudokuBoardBlockWidth;
        int BlockCenterRow = SudokuBoardBlockHeight / 2;
        int BlockCenterColumn = SudokuBoardBlockWidth / 2;

        if (RemainingNumbersForSudokuFieldsFormingPreemptiveSetInCenterDotRegion.size() > 0
            && SudokuFieldsFormingPreemptiveSetInCenterDotRegion.size() > 0) {
            for (int Iterator1 = 0; Iterator1 < BlocksRows; Iterator1++) {
                for (int Iterator2 = 0; Iterator2 < BlocksColumns; Iterator2++) {
                    if (!CheckIfContainsSudokuField(SudokuFieldsFormingPreemptiveSetInCenterDotRegion,
                        Iterator1 * SudokuBoardBlockHeight + BlockCenterRow,
                        Iterator2 * SudokuBoardBlockWidth + BlockCenterColumn)) {
                        RemainingNumbersForSudokuFields[Iterator1 * SudokuBoardBlockHeight + BlockCenterRow]
                            [Iterator2 * SudokuBoardBlockWidth + BlockCenterColumn].removeAll(
                            RemainingNumbersForSudokuFieldsFormingPreemptiveSetInCenterDotRegion);
                    }
                }
            }
        }
    }

    /**
     * Gets preemptive sets in relevant rows, in relevant columns, in relevant blocks, etc. and updates remaining
     * numbers for sudoku fields (all possible numbers to fill sudoku fields).
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns array of sets containing deleted numbers for relevant sudoku fields (needed for backtracking).
     */

    private HashSet<Integer>[][] GetPreemptiveSetsAndUpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSets(
        int[][] SudokuBoardArray, HashSet<Integer>[][] RemainingNumbersForSudokuFields, String SudokuType) {
        HashSet<Integer>[][] DeletedNumbersForSudokuFields = new HashSet[RemainingNumbersForSudokuFields.length]
            [RemainingNumbersForSudokuFields.length];

        for (int Iterator1 = 0; Iterator1 < DeletedNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < DeletedNumbersForSudokuFields.length; Iterator2++) {
                DeletedNumbersForSudokuFields[Iterator1][Iterator2] = new HashSet<>(RemainingNumbersForSudokuFields
                    [Iterator1][Iterator2]);
            }
        }

        for (int Iterator = 2; Iterator < SudokuBoardArray.length; Iterator++) {
            HashSet<SudokuField>[] NotFilledSudokuFieldsInRows = GetNotFilledSudokuFieldsInRows(SudokuBoardArray);
            PreemptiveSet[] PreemptiveSetsInRows = GetPreemptiveSets(RemainingNumbersForSudokuFields,
                NotFilledSudokuFieldsInRows, Iterator);
            UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInRows(RemainingNumbersForSudokuFields,
                PreemptiveSetsInRows);
            HashSet<SudokuField>[] NotFilledSudokuFieldsInColumns = GetNotFilledSudokuFieldsInColumns(SudokuBoardArray);
            PreemptiveSet[] PreemptiveSetsInColumns = GetPreemptiveSets(RemainingNumbersForSudokuFields,
                NotFilledSudokuFieldsInColumns, Iterator);
            UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInColumns(RemainingNumbersForSudokuFields,
                PreemptiveSetsInColumns);
            HashSet<SudokuField>[] NotFilledSudokuFieldsInBlocks = GetNotFilledSudokuFieldsInBlocks(SudokuBoardArray);
            PreemptiveSet[] PreemptiveSetsInBlocks = GetPreemptiveSets(RemainingNumbersForSudokuFields,
                NotFilledSudokuFieldsInBlocks, Iterator);
            UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInBlocks(RemainingNumbersForSudokuFields,
                PreemptiveSetsInBlocks);

            switch (SudokuType) {
                case "Diagonal":
                    HashSet<SudokuField>[] NotFilledSudokuFieldsInDiagonals = GetNotFilledSudokuFieldsInDiagonals(
                        SudokuBoardArray);
                    PreemptiveSet[] PreemptiveSetsInDiagonals = GetPreemptiveSets(RemainingNumbersForSudokuFields,
                        NotFilledSudokuFieldsInDiagonals, Iterator);
                    UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInDiagonals(RemainingNumbersForSudokuFields,
                        PreemptiveSetsInDiagonals);
                    break;
                case "Offset":
                    HashSet<SudokuField>[] NotFilledSudokuFieldsInOffsetRegions
                        = GetNotFilledSudokuFieldsInOffsetRegions(SudokuBoardArray);
                    PreemptiveSet[] PreemptiveSetsInOffsetRegions = GetPreemptiveSets(RemainingNumbersForSudokuFields,
                        NotFilledSudokuFieldsInOffsetRegions, Iterator);
                    UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInOffsetRegions(
                        RemainingNumbersForSudokuFields, PreemptiveSetsInOffsetRegions);
                    break;
                case "Windoku":
                    HashSet<SudokuField>[] NotFilledSudokuFieldsInWindokuRegions
                        = GetNotFilledSudokuFieldsInWindokuRegions(SudokuBoardArray);
                    PreemptiveSet[] PreemptiveSetsInWindokuRegions = GetPreemptiveSets(RemainingNumbersForSudokuFields,
                        NotFilledSudokuFieldsInWindokuRegions, Iterator);
                    UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInWindokuRegions(
                        RemainingNumbersForSudokuFields, PreemptiveSetsInWindokuRegions);
                    break;
                case "Center Dot":
                    HashSet<SudokuField>[] NotFilledSudokuFieldsInCenterDotRegion
                        = GetNotFilledSudokuFieldsInCenterDotRegion(SudokuBoardArray);
                    PreemptiveSet[] PreemptiveSetsInCenterDotRegion = GetPreemptiveSets(RemainingNumbersForSudokuFields,
                        NotFilledSudokuFieldsInCenterDotRegion, Iterator);
                    UpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSetsInCenterDotRegion(
                        RemainingNumbersForSudokuFields, PreemptiveSetsInCenterDotRegion);
                    break;
            }
        }

        for (int Iterator1 = 0; Iterator1 < DeletedNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < DeletedNumbersForSudokuFields.length; Iterator2++) {
                DeletedNumbersForSudokuFields[Iterator1][Iterator2].removeAll(RemainingNumbersForSudokuFields[Iterator1]
                    [Iterator2]);
            }
        }

        return DeletedNumbersForSudokuFields;
    }

    /**
     * Updates remaining numbers for sudoku fields (all possible numbers to fill sudoku fields - needed for
     * backtracking).
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param DeletedNumbersForSudokuFieldsForUniqueRemainingNumber Parameter is array of sets containing deleted
     * numbers for relevant sudoku fields obtained after finding unique remaining numbers in relevant rows, in relevant
     * columns, in relevant blocks, etc.
     * @param DeletedNumbersForSudokuFieldsForPreemptiveSets Parameter is array of sets containing deleted numbers for
     * relevant sudoku fields obtained after finding preemptive sets in relevant rows, in relevant columns, in relevant
     * blocks, etc.
     */

    private void UpdateRemainingNumbersForSudokuFields(HashSet<Integer>[][] RemainingNumbersForSudokuFields,
        HashSet<Integer>[][] DeletedNumbersForSudokuFieldsForUniqueRemainingNumber,
        HashSet<Integer>[][] DeletedNumbersForSudokuFieldsForPreemptiveSets) {
        for (int Iterator1 = 0; Iterator1 < RemainingNumbersForSudokuFields.length; Iterator1++) {
            for (int Iterator2 = 0; Iterator2 < RemainingNumbersForSudokuFields.length; Iterator2++) {
                RemainingNumbersForSudokuFields[Iterator1][Iterator2].addAll(
                    DeletedNumbersForSudokuFieldsForUniqueRemainingNumber[Iterator1][Iterator2]);
                RemainingNumbersForSudokuFields[Iterator1][Iterator2].addAll(
                    DeletedNumbersForSudokuFieldsForPreemptiveSets[Iterator1][Iterator2]);
            }
        }
    }

    // long CountCheckedSudokuStatesForCrookAlgorithm = 0;

    /**
     * Solves given sudoku board using Crook's algorithm.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     * @return Returns if given sudoku board has been solved.
     */

    private boolean SolveSudokuBoardUsingCrookAlgorithm(int[][] SudokuBoardArray,
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, String SudokuType) {
        if (Thread.currentThread().isInterrupted()) {
            throw new RuntimeException();
        }

        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            = GetNotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(SudokuBoardArray,
            RemainingNumbersForSudokuFields);

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.isEmpty()
            && NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == DEFAULT_VALUE) {
            return true;
        }

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 0) {
            return false;
        }

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 1) {
            SudokuField NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (SudokuField)
                NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.toArray()[0];
            int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow
                = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldRow;
            int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn
                = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldColumn;
            int RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers
                = (int) RemainingNumbersForSudokuFields
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn].toArray()[0];
            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers;
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField = UpdateRemainingNumbersForSudokuFields(
                RemainingNumbersForSudokuFields, NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers, SudokuType);
            HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields = new HashSet<>();

            if (SudokuType.equals("Non-Consecutive")) {
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                    = UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                    RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            }

            // CountCheckedSudokuStatesForCrookAlgorithm++;

            if (SolveSudokuBoardUsingCrookAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields, SudokuType)) {
                return true;
            }

            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);

            if (SudokuType.equals("Non-Consecutive")) {
                UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields);
            }

            return false;
        }

        HashSet<Integer>[][] DeletedNumbersForSudokuFieldsForUniqueRemainingNumber
            = GetSudokuFieldsWithUniqueRemainingNumberAndUpdateRemainingNumbersForSudokuFields(
            RemainingNumbersForSudokuFields, SudokuType);
        HashSet<Integer>[][] DeletedNumbersForSudokuFieldsForPreemptiveSets
            = GetPreemptiveSetsAndUpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSets(SudokuBoardArray,
            RemainingNumbersForSudokuFields, SudokuType);
        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            = GetNotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(SudokuBoardArray,
            RemainingNumbersForSudokuFields);

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 0) {
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                DeletedNumbersForSudokuFieldsForUniqueRemainingNumber, DeletedNumbersForSudokuFieldsForPreemptiveSets);
            return false;
        }

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 1) {
            SudokuField NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (SudokuField)
                NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.toArray()[0];
            int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow
                = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldRow;
            int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn
                = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldColumn;
            int RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers
                = (int) RemainingNumbersForSudokuFields
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn].toArray()[0];
            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers;
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField = UpdateRemainingNumbersForSudokuFields(
                RemainingNumbersForSudokuFields, NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers, SudokuType);
            HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields = new HashSet<>();

            if (SudokuType.equals("Non-Consecutive")) {
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                    = UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                    RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            }

            // CountCheckedSudokuStatesForCrookAlgorithm++;

            if (SolveSudokuBoardUsingCrookAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields, SudokuType)) {
                return true;
            }

            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                DeletedNumbersForSudokuFieldsForUniqueRemainingNumber, DeletedNumbersForSudokuFieldsForPreemptiveSets);

            if (SudokuType.equals("Non-Consecutive")) {
                UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields);
            }

            return false;
        }

        SudokuField NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (SudokuField)
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.toArray()[0];
        int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow
            = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldRow;
        int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn
            = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldColumn;
        HashSet<Integer> RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers
            = RemainingNumbersForSudokuFields[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
            [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn];

        for (int Iterator = 0; Iterator
            < RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.size(); Iterator++) {
            int RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (int)
                RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.toArray()[Iterator];
            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers;
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField = UpdateRemainingNumbersForSudokuFields(
                RemainingNumbersForSudokuFields, NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers, SudokuType);
            HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields = new HashSet<>();

            if (SudokuType.equals("Non-Consecutive")) {
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                    = UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                    RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            }

            // CountCheckedSudokuStatesForCrookAlgorithm++;

            if (SolveSudokuBoardUsingCrookAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields, SudokuType)) {
                return true;
            }

            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);

            if (SudokuType.equals("Non-Consecutive")) {
                UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields);
            }
        }

        UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
            DeletedNumbersForSudokuFieldsForUniqueRemainingNumber, DeletedNumbersForSudokuFieldsForPreemptiveSets);
        return false;
    }

    /**
     * Solves given sudoku board using Crook's algorithm.
     * Usage: /SolveUsingCrookAlgorithm?SudokuBoardSize={SudokuBoardSize}&amp;SudokuType={SudokuType}
     * &amp;SudokuBoard={SudokuBoard}
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @param SudokuType Parameter is sudoku type.
     * @param SudokuBoard Parameter is sudoku board to be checked.
     * @return Returns information about solution of sudoku board obtained by using Crook's algorithm.
     */

    @GetMapping(path = "/SolveUsingCrookAlgorithm", produces = MediaType.APPLICATION_JSON_VALUE)
    public String SolveSudokuBoardUsingCrookAlgorithm(@RequestParam int SudokuBoardSize,
        @RequestParam String SudokuType, @RequestParam String SudokuBoard) {
        ExecutorService ExecutorService = Executors.newSingleThreadExecutor();

        Callable<String> SolveSudokuBoardUsingCrookAlgorithmCallable = () -> {
            String CheckIfGivenSudokuBoardIsValid = CheckIfGivenSudokuBoardIsValid(SudokuBoardSize, SudokuType,
                SudokuBoard);
            JSONObject CheckIfGivenSudokuBoardIsValidMessage = new JSONObject(CheckIfGivenSudokuBoardIsValid);
            String CheckIfGivenSudokuBoardIsValidMessageResult = CheckIfGivenSudokuBoardIsValidMessage.getString(
                JSON_RESULT);

            if (CheckIfGivenSudokuBoardIsValidMessageResult.equals(JSON_RESULT_SUCCESS)) {
                int[][] SudokuBoardArray = SudokuBoardTo2DArrayOfIntegers(SudokuBoardSize, SudokuBoard);
                HashSet<Integer>[][] RemainingNumbersForSudokuFields = GetRemainingNumbersForSudokuFields(
                    SudokuBoardArray, SudokuType);

                // long StartTime = System.nanoTime();
                // CountCheckedSudokuStatesForCrookAlgorithm = 0;

                if (SolveSudokuBoardUsingCrookAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields,
                    SudokuType)) {
                    // long StopTime = System.nanoTime();
                    // System.out.println((double) (StopTime - StartTime) / 1000000000);
                    // System.out.println(CountCheckedSudokuStatesForCrookAlgorithm);

                    String SolvedSudokuBoard = SudokuBoardToString(SudokuBoardArray);
                    String CheckIfGivenSolvedSudokuBoardIsValid = CheckIfGivenSudokuBoardIsValid(SudokuBoardSize,
                        SudokuType, SolvedSudokuBoard);
                    JSONObject CheckIfGivenSolvedSudokuBoardIsValidMessage = new JSONObject(
                        CheckIfGivenSolvedSudokuBoardIsValid);
                    String CheckIfGivenSolvedSudokuBoardIsValidMessageResult
                        = CheckIfGivenSolvedSudokuBoardIsValidMessage.getString(JSON_RESULT);

                    if (CheckIfGivenSolvedSudokuBoardIsValidMessageResult.equals(JSON_RESULT_SUCCESS)) {
                        JSONObject SuccessMessage = new JSONObject();
                        SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                        SuccessMessage.put(JSON_INFO, SolvedSudokuBoard);
                        return SuccessMessage.toString();
                    }

                    JSONObject ErrorMessage = new JSONObject();
                    ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                    ErrorMessage.put(JSON_INFO, "Sudoku board has no solution! ");
                    return ErrorMessage.toString();
                }

                JSONObject ErrorMessage = new JSONObject();
                ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                ErrorMessage.put(JSON_INFO, "Sudoku board has no solution! ");
                return ErrorMessage.toString();
            } else {
                return CheckIfGivenSudokuBoardIsValid;
            }
        };

        Future<String> SolveSudokuBoardUsingCrookAlgorithmFuture = ExecutorService.submit(
            SolveSudokuBoardUsingCrookAlgorithmCallable);

        try {
            String SuccessMessage = SolveSudokuBoardUsingCrookAlgorithmFuture.get(15, TimeUnit.SECONDS);
            ExecutorService.shutdown();
            return SuccessMessage;
        } catch (InterruptedException | ExecutionException Exception) {
            SolveSudokuBoardUsingCrookAlgorithmFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Error during execution! ");
            return ErrorMessage.toString();
        } catch (TimeoutException Exception) {
            SolveSudokuBoardUsingCrookAlgorithmFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Timeout! ");
            return ErrorMessage.toString();
        }
    }

    /**
     * Gets number of solutions of given sudoku board using Wave Function Collapse algorithm.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     */

    private void GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithm(int[][] SudokuBoardArray,
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, String SudokuType) {
        if (Thread.currentThread().isInterrupted()) {
            throw new RuntimeException();
        }

        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            = GetNotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(SudokuBoardArray,
            RemainingNumbersForSudokuFields);

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.isEmpty()
            && NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == DEFAULT_VALUE) {
            NumberOfSudokuSolutionsUsingWaveFunctionCollapseAlgorithm++;
            return;
        }

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 0) {
            return;
        }

        SudokuField NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (SudokuField)
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.toArray()[0];
        int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow
            = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldRow;
        int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn
            = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldColumn;
        HashSet<Integer> RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers
            = RemainingNumbersForSudokuFields[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
            [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn];

        for (int Iterator = 0; Iterator
            < RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.size(); Iterator++) {
            int RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (int)
                RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.toArray()[Iterator];
            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers;
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField = UpdateRemainingNumbersForSudokuFields(
                RemainingNumbersForSudokuFields, NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers, SudokuType);
            HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields = new HashSet<>();

            if (SudokuType.equals("Non-Consecutive")) {
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                    = UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                    RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            }

            GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields,
                SudokuType);

            if (NumberOfSudokuSolutionsUsingWaveFunctionCollapseAlgorithm >= GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT) {
                return;
            }

            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);

            if (SudokuType.equals("Non-Consecutive")) {
                UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields);
            }
        }
    }

    /**
     * Gets number of solutions of given sudoku board using Crook's algorithm.
     * @param SudokuBoardArray Parameter is sudoku board to be solved.
     * @param RemainingNumbersForSudokuFields Parameter is array of sets containing remaining numbers for relevant
     * sudoku fields.
     * @param SudokuType Parameter is sudoku type.
     */

    private void GetNumberOfSolutionsUsingCrookAlgorithm(int[][] SudokuBoardArray,
        HashSet<Integer>[][] RemainingNumbersForSudokuFields, String SudokuType) {
        if (Thread.currentThread().isInterrupted()) {
            throw new RuntimeException();
        }

        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            = GetNotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(SudokuBoardArray,
            RemainingNumbersForSudokuFields);

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.isEmpty()
            && NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == DEFAULT_VALUE) {
            NumberOfSudokuSolutionsUsingCrookAlgorithm++;
            return;
        }

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 0) {
            return;
        }

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 1) {
            SudokuField NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (SudokuField)
                NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.toArray()[0];
            int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow
                = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldRow;
            int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn
                = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldColumn;
            int RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers
                = (int) RemainingNumbersForSudokuFields
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn].toArray()[0];
            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers;
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField = UpdateRemainingNumbersForSudokuFields(
                RemainingNumbersForSudokuFields, NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers, SudokuType);
            HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields = new HashSet<>();

            if (SudokuType.equals("Non-Consecutive")) {
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                    = UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                    RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            }

            GetNumberOfSolutionsUsingCrookAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields, SudokuType);

            if (NumberOfSudokuSolutionsUsingCrookAlgorithm >= GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT) {
                return;
            }

            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);

            if (SudokuType.equals("Non-Consecutive")) {
                UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields);
            }

            return;
        }

        HashSet<Integer>[][] DeletedNumbersForSudokuFieldsForUniqueRemainingNumber
            = GetSudokuFieldsWithUniqueRemainingNumberAndUpdateRemainingNumbersForSudokuFields(
            RemainingNumbersForSudokuFields, SudokuType);
        HashSet<Integer>[][] DeletedNumbersForSudokuFieldsForPreemptiveSets
            = GetPreemptiveSetsAndUpdateRemainingNumbersForSudokuFieldsUsingPreemptiveSets(SudokuBoardArray,
            RemainingNumbersForSudokuFields, SudokuType);
        NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            = GetNotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers(SudokuBoardArray,
            RemainingNumbersForSudokuFields);

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 0) {
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                DeletedNumbersForSudokuFieldsForUniqueRemainingNumber, DeletedNumbersForSudokuFieldsForPreemptiveSets);
            return;
        }

        if (NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .MinimumNumberOfRemainingNumbersForNotFilledSudokuFields == 1) {
            SudokuField NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (SudokuField)
                NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
                .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.toArray()[0];
            int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow
                = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldRow;
            int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn
                = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldColumn;
            int RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers
                = (int) RemainingNumbersForSudokuFields
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn].toArray()[0];
            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers;
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField = UpdateRemainingNumbersForSudokuFields(
                RemainingNumbersForSudokuFields, NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers, SudokuType);
            HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields = new HashSet<>();

            if (SudokuType.equals("Non-Consecutive")) {
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                    = UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                    RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            }

            GetNumberOfSolutionsUsingCrookAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields, SudokuType);

            if (NumberOfSudokuSolutionsUsingCrookAlgorithm >= GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT) {
                return;
            }

            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                DeletedNumbersForSudokuFieldsForUniqueRemainingNumber, DeletedNumbersForSudokuFieldsForPreemptiveSets);

            if (SudokuType.equals("Non-Consecutive")) {
                UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields);
            }

            return;
        }

        SudokuField NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (SudokuField)
            NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers
            .NotFilledSudokuFieldsWithMinimumNumberOfRemainingNumbers.toArray()[0];
        int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow
            = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldRow;
        int NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn
            = NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.SudokuFieldColumn;
        HashSet<Integer> RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers
            = RemainingNumbersForSudokuFields[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
            [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn];

        for (int Iterator = 0; Iterator
            < RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.size(); Iterator++) {
            int RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers = (int)
                RemainingNumbersForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers.toArray()[Iterator];
            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers;
            UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField = UpdateRemainingNumbersForSudokuFields(
                RemainingNumbersForSudokuFields, NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers, SudokuType);
            HashSet<UpdatedSudokuFieldAndDeletedNumberForSudokuField>
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields = new HashSet<>();

            if (SudokuType.equals("Non-Consecutive")) {
                UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields
                    = UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                    NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                    RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);
            }

            GetNumberOfSolutionsUsingCrookAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields, SudokuType);

            if (NumberOfSudokuSolutionsUsingCrookAlgorithm >= GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT) {
                return;
            }

            SudokuBoardArray[NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow]
                [NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn]
                = DEFAULT_SUDOKU_FIELD_NOT_FILLED_VALUE;
            UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
                UpdatedSudokuFieldsAndRemainingNumbersForFilledSudokuField,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldRow,
                NotFilledSudokuFieldWithMinimumNumberOfRemainingNumbersSudokuFieldColumn,
                RemainingNumberForNotFilledSudokuFieldWithMinimumNumberOfRemainingNumbers);

            if (SudokuType.equals("Non-Consecutive")) {
                UpdateRemainingNumbersForSudokuFieldsInNonConsecutiveType(RemainingNumbersForSudokuFields,
                    UpdatedSudokuFieldsAndDeletedNumbersForSudokuFields);
            }
        }

        UpdateRemainingNumbersForSudokuFields(RemainingNumbersForSudokuFields,
            DeletedNumbersForSudokuFieldsForUniqueRemainingNumber, DeletedNumbersForSudokuFieldsForPreemptiveSets);
    }

    /**
     * Gets number of solutions of given sudoku board (using slightly modified Crook's algorithm).
     * Usage: /GetNumberOfSolutions?SudokuBoardSize={SudokuBoardSize}&amp;SudokuType={SudokuType}
     * &amp;SudokuBoard={SudokuBoard}
     * @param SudokuBoardSize Parameter is sudoku board size.
     * @param SudokuType Parameter is sudoku type.
     * @param SudokuBoard Parameter is sudoku board to be checked.
     * @return Returns information about number of solutions of sudoku board.
     */

    @GetMapping(path = "/GetNumberOfSolutions", produces = MediaType.APPLICATION_JSON_VALUE)
    public String GetNumberOfSolutions(@RequestParam int SudokuBoardSize, @RequestParam String SudokuType,
        @RequestParam String SudokuBoard) {
        ExecutorService ExecutorService = Executors.newFixedThreadPool(2);

        Callable<String> GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmCallable = () -> {
            String CheckIfGivenSudokuBoardIsValid = CheckIfGivenSudokuBoardIsValid(SudokuBoardSize, SudokuType,
                SudokuBoard);
            JSONObject CheckIfGivenSudokuBoardIsValidMessage = new JSONObject(CheckIfGivenSudokuBoardIsValid);
            String CheckIfGivenSudokuBoardIsValidMessageResult = CheckIfGivenSudokuBoardIsValidMessage.getString(
                JSON_RESULT);

            if (CheckIfGivenSudokuBoardIsValidMessageResult.equals(JSON_RESULT_SUCCESS)) {
                int[][] SudokuBoardArray = SudokuBoardTo2DArrayOfIntegers(SudokuBoardSize, SudokuBoard);
                HashSet<Integer>[][] RemainingNumbersForSudokuFields = GetRemainingNumbersForSudokuFields(
                    SudokuBoardArray, SudokuType);
                NumberOfSudokuSolutionsUsingWaveFunctionCollapseAlgorithm = 0;
                GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithm(SudokuBoardArray,
                    RemainingNumbersForSudokuFields, SudokuType);

                if (NumberOfSudokuSolutionsUsingWaveFunctionCollapseAlgorithm == 0) {
                    JSONObject ErrorMessage = new JSONObject();
                    ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                    ErrorMessage.put(JSON_INFO, "Sudoku board has no solution! ");
                    return ErrorMessage.toString();
                } else if (NumberOfSudokuSolutionsUsingWaveFunctionCollapseAlgorithm == 1) {
                    JSONObject SuccessMessage = new JSONObject();
                    SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                    SuccessMessage.put(JSON_INFO, "Sudoku board has a unique solution! ");
                    return SuccessMessage.toString();
                } else if (NumberOfSudokuSolutionsUsingWaveFunctionCollapseAlgorithm
                    < GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT) {
                    JSONObject SuccessMessage = new JSONObject();
                    SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                    SuccessMessage.put(JSON_INFO, "Sudoku board has "
                        + NumberOfSudokuSolutionsUsingWaveFunctionCollapseAlgorithm + " solutions! ");
                    return SuccessMessage.toString();
                } else {
                    JSONObject SuccessMessage = new JSONObject();
                    SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                    SuccessMessage.put(JSON_INFO, "Sudoku board has at least " + GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT
                        + " solutions! ");
                    return SuccessMessage.toString();
                }
            } else {
                return CheckIfGivenSudokuBoardIsValid;
            }
        };

        Callable<String> GetNumberOfSolutionsUsingCrookAlgorithmCallable = () -> {
            String CheckIfGivenSudokuBoardIsValid = CheckIfGivenSudokuBoardIsValid(SudokuBoardSize, SudokuType,
                SudokuBoard);
            JSONObject CheckIfGivenSudokuBoardIsValidMessage = new JSONObject(CheckIfGivenSudokuBoardIsValid);
            String CheckIfGivenSudokuBoardIsValidMessageResult = CheckIfGivenSudokuBoardIsValidMessage.getString(
                JSON_RESULT);

            if (CheckIfGivenSudokuBoardIsValidMessageResult.equals(JSON_RESULT_SUCCESS)) {
                int[][] SudokuBoardArray = SudokuBoardTo2DArrayOfIntegers(SudokuBoardSize, SudokuBoard);
                HashSet<Integer>[][] RemainingNumbersForSudokuFields = GetRemainingNumbersForSudokuFields(
                    SudokuBoardArray, SudokuType);
                NumberOfSudokuSolutionsUsingCrookAlgorithm = 0;
                GetNumberOfSolutionsUsingCrookAlgorithm(SudokuBoardArray, RemainingNumbersForSudokuFields, SudokuType);

                if (NumberOfSudokuSolutionsUsingCrookAlgorithm == 0) {
                    JSONObject ErrorMessage = new JSONObject();
                    ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
                    ErrorMessage.put(JSON_INFO, "Sudoku board has no solution! ");
                    return ErrorMessage.toString();
                } else if (NumberOfSudokuSolutionsUsingCrookAlgorithm == 1) {
                    JSONObject SuccessMessage = new JSONObject();
                    SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                    SuccessMessage.put(JSON_INFO, "Sudoku board has a unique solution! ");
                    return SuccessMessage.toString();
                } else if (NumberOfSudokuSolutionsUsingCrookAlgorithm < GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT) {
                    JSONObject SuccessMessage = new JSONObject();
                    SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                    SuccessMessage.put(JSON_INFO, "Sudoku board has " + NumberOfSudokuSolutionsUsingCrookAlgorithm
                        + " solutions! ");
                    return SuccessMessage.toString();
                } else {
                    JSONObject SuccessMessage = new JSONObject();
                    SuccessMessage.put(JSON_RESULT, JSON_RESULT_SUCCESS);
                    SuccessMessage.put(JSON_INFO, "Sudoku board has at least " + GET_NUMBER_OF_SUDOKU_SOLUTIONS_LIMIT
                        + " solutions! ");
                    return SuccessMessage.toString();
                }
            } else {
                return CheckIfGivenSudokuBoardIsValid;
            }
        };

        Future<String> GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmFuture = ExecutorService.submit(
            GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmCallable);
        Future<String> GetNumberOfSolutionsUsingCrookAlgorithmFuture = ExecutorService.submit(
            GetNumberOfSolutionsUsingCrookAlgorithmCallable);

        try {
            long GetNumberOfSolutionsStartTime = System.nanoTime();
            long GetNumberOfSolutionsTimeout = 20000000000L;

            while (System.nanoTime() - GetNumberOfSolutionsStartTime < GetNumberOfSolutionsTimeout) {
                if (GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmFuture.isDone()
                    || GetNumberOfSolutionsUsingCrookAlgorithmFuture.isDone()) {
                    break;
                }
            }

            if (GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmFuture.isDone()) {
                GetNumberOfSolutionsUsingCrookAlgorithmFuture.cancel(true);
                String SuccessMessage = GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmFuture.get();
                ExecutorService.shutdown();
                return SuccessMessage;
            }

            if (GetNumberOfSolutionsUsingCrookAlgorithmFuture.isDone()) {
                GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmFuture.cancel(true);
                String SuccessMessage = GetNumberOfSolutionsUsingCrookAlgorithmFuture.get();
                ExecutorService.shutdown();
                return SuccessMessage;
            }

            throw new TimeoutException();
        } catch (InterruptedException | ExecutionException Exception) {
            GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmFuture.cancel(true);
            GetNumberOfSolutionsUsingCrookAlgorithmFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Error during execution! ");
            return ErrorMessage.toString();
        } catch (TimeoutException Exception) {
            GetNumberOfSolutionsUsingWaveFunctionCollapseAlgorithmFuture.cancel(true);
            GetNumberOfSolutionsUsingCrookAlgorithmFuture.cancel(true);
            ExecutorService.shutdown();
            JSONObject ErrorMessage = new JSONObject();
            ErrorMessage.put(JSON_RESULT, JSON_RESULT_ERROR);
            ErrorMessage.put(JSON_INFO, "Timeout! ");
            return ErrorMessage.toString();
        }
    }
}
