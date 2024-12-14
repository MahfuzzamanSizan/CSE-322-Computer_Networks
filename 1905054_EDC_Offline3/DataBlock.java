public class DataBlock {


    public static String Ascii_String_computation(String[][] dataBlock) {
        int numRows = dataBlock.length;
        int numCols = dataBlock[0].length;

        StringBuilder asciiDataString = new StringBuilder();

        for (int col = 0; col < numCols; col++) {
            for (int row = 0; row < numRows; row++) {
                String binary = dataBlock[row][col];
                char character = (char) Integer.parseInt(binary, 2);
                asciiDataString.append(character);
            }
        }

        return asciiDataString.toString();
    }

    public static String[][] Data_Block_With_Check_Bits(String dataString, int m) {
        int numRows = dataString.length() / m;
        String[][] dataBlock = new String[numRows][m + 4];

        int dataIndex = 0;

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < m; col++) {
                char character = dataString.charAt(dataIndex++);
                String binary = String.format("%8s", Integer.toBinaryString((int) character)).replace(' ', '0');
                dataBlock[row][col] = binary;
            }


            String checkBits = CRC.Check_Bits_Calculation(dataBlock[row]);
            for (int i = 0; i < 4; i++) {
                dataBlock[row][m + i] = checkBits.charAt(i) + "";
            }
        }


        System.out.println("Data Block after adding Check Bits:");
        for (String[] row : dataBlock) {
            for (String binary : row) {
                System.out.print(binary + " ");
            }
            System.out.println();
        }

        return dataBlock;
    }


    public static String Data_Block_Serialization(String[][] dataBlock) {
        StringBuilder serializedData = new StringBuilder();
        int numRows = dataBlock.length;
        int numCols = dataBlock[0].length;

        for (int col = 0; col < numCols; col++) {
            for (int row = 0; row < numRows; row++) {
                String binary = dataBlock[row][col];
                serializedData.append(binary);
            }
        }
        return serializedData.toString();
    }

    public static String[][] Data_Block_Deserialization(String serializedData, int numRows, int numCols) {
        String[][] dataBlock = new String[numRows][numCols];
        int bitIndex = 0;

        for (int col = 0; col < numCols; col++) {
            for (int row = 0; row < numRows; row++) {

                if (bitIndex + 8 <= serializedData.length()) {

                    String binary = serializedData.substring(bitIndex, bitIndex + 8);
                    bitIndex += 8;
                    dataBlock[row][col] = binary;
                } else {

                    dataBlock[row][col] = "00000000";
                }
            }
        }

        return dataBlock;
    }



}
