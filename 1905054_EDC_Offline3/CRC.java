import java.util.Random;

public class CRC {
    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_CYAN = "\u001B[36m";
    public static final String ANSI_RED = "\u001B[31m";


    public static String Flip_Bit(String bit) {
        return bit.equals("0") ? "1" : "0";
    }

    public static String Data_String_Padding(String dataString, int m) {
        int paddingLength = m - (dataString.length() % m);
        if (paddingLength > 0) {
            StringBuilder paddedData = new StringBuilder(dataString);
            for (int i = 0; i < paddingLength; i++) {
                paddedData.append("~");
            }
            dataString = paddedData.toString();
        }
        System.out.println("Updated Data String (Padded): " + dataString);
        return dataString;
    }


    public static String Check_Bits_Calculation(String[] dataRow) {
        int numCols = dataRow.length;
        String dataWithoutCheckBits = String.join("", dataRow);

        int[] generator = {1, 0, 1, 1};


        String dataWithCheckBits = dataWithoutCheckBits + "0000";

        char[] dataChars = dataWithCheckBits.toCharArray();

        for (int i = 0; i < dataWithoutCheckBits.length(); i++) {
            if (dataChars[i] == '1') {

                for (int j = 0; j < generator.length; j++) {
                    dataChars[i + j] = (dataChars[i + j] == generator[j]) ? '0' : '1';
                }
            }
        }

        return new String(dataChars, numCols, 4);
    }




    public static String Transmission_Simulatation(String frame, double p) {
        StringBuilder receivedFrame = new StringBuilder();
        Random random = new Random();

        for (char bit : frame.toCharArray()) {
            if (random.nextDouble() <= p) {

                receivedFrame.append(bit == '0' ? '1' : '0');
            } else {
                receivedFrame.append(bit);
            }
        }

        return receivedFrame.toString();
    }


    public static String CRC_Calculation(String data, String generatorPolynomial) {
        int generatorLength = generatorPolynomial.length();
        char[] dataChars = data.toCharArray();
        char[] generatorChars = generatorPolynomial.toCharArray();
        char[] remainder = new char[generatorLength];
        char[] divisor = new char[generatorLength];


        for (int i = 0; i < generatorLength; i++) {
            remainder[i] = dataChars[i];
        }


        for (int i = generatorLength; i < dataChars.length; i++) {
            if (remainder[0] == '1') {
                for (int j = generatorLength - 1; j >= 0; j--) {
                    divisor[j] = generatorChars[j];
                }
                for (int j = generatorLength - 1; j >= 0; j--) {
                    if (remainder[j] == divisor[j]) {
                        remainder[j] = '0';
                    } else {
                        remainder[j] = '1';
                    }
                }
            } else {
                for (int j = generatorLength - 1; j > 0; j--) {
                    remainder[j] = remainder[j - 1];
                }
                remainder[0] = dataChars[i];
            }
        }

        return new String(remainder);
    }





    public static void Errors_Correction_In_Data_Block(String[][] dataBlock) {
        int numRows = dataBlock.length;
        int numCols = dataBlock[0].length - 4;

        for (int row = 0; row < numRows; row++) {
            int errorCount = 0;


            int[] errorColumns = new int[numCols];
            for (int col = 0; col < numCols; col++) {
                String binary = dataBlock[row][col];
                int parity = Parity_Calculation(dataBlock[row], numCols);
                if (!binary.equals(Integer.toBinaryString(parity))) {
                    errorColumns[errorCount] = col;
                    errorCount++;
                }
            }


            if (errorCount == 1) {
                int errorColumn = errorColumns[0];
                dataBlock[row][errorColumn] = Flip_Bit(dataBlock[row][errorColumn]);
            }
        }
    }


    public static String[][] Check_Bits_removing(String[][] dataBlock) {
        int numRows = dataBlock.length;
        int numCols = dataBlock[0].length - 4;
        String[][] correctedDataBlock = new String[numRows][numCols];

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                correctedDataBlock[row][col] = dataBlock[row][col];
            }
        }

        return correctedDataBlock;
    }



    public static int Parity_Calculation(String[] dataRow, int numCols) {
        int parity = 0;

        for (int col = 0; col < numCols; col++) {
            String binary = dataRow[col];

            while (binary.length() < 8) {
                binary = "0" + binary;
            }
            parity ^= Integer.parseInt(binary, 2);
        }

        return parity;
    }


}
