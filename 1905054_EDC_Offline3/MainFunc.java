import java.util.Scanner;

public class MainFunc {
    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter the data string to be transmitted: ");
        String data_string = scanner.nextLine();


        System.out.print("Enter the number of bytes in a row of the data block (m): ");
        int m = scanner.nextInt();


        System.out.print("Enter the probability of each bit being toggled during transmission (p): ");
        double p = scanner.nextDouble();


        scanner.nextLine();
        System.out.print("Enter the generator polynomial: ");
        String generator_polynomial = scanner.nextLine();

        scanner.close();


        data_string = CRC.Data_String_Padding(data_string, m);


        String[][] data_block = DataBlock.Data_Block_With_Check_Bits(data_string, m);


        String serialized_data = DataBlock.Data_Block_Serialization(data_block);


        System.out.println("Sent Frame (Serialized Data): " + serialized_data);


        String crc_checksum = CRC.CRC_Calculation(serialized_data, generator_polynomial);


        String frame = serialized_data + crc_checksum;


        String received_frame = CRC.Transmission_Simulatation(frame, p);

        System.out.println("Received Frame: " + received_frame);


        String received_CRC = received_frame.substring(received_frame.length() - crc_checksum.length());


        String received_data_without_CRC = received_frame.substring(0, received_frame.length() - crc_checksum.length());


        String[][] received_data_block = DataBlock.Data_Block_Deserialization(received_data_without_CRC, data_block.length, data_block[0].length);


        CRC.Errors_Correction_In_Data_Block(received_data_block);


        String[][] corrected_data_block = CRC.Check_Bits_removing(received_data_block);


        System.out.println("Data Block After Removing CRC Checksum Bits and Check Bits:");
        for (String[] row : corrected_data_block) {
            for (String binary : row) {
                System.out.print(binary + " ");
            }
            System.out.println();
        }


        String ascii_data_string = DataBlock.Ascii_String_computation(corrected_data_block);


        System.out.println("Data String: " + ascii_data_string);


        String corrected_data_without_CRC = DataBlock.Data_Block_Serialization(corrected_data_block);
        String calculated_CRC = CRC.CRC_Calculation(corrected_data_without_CRC, generator_polynomial);


        boolean error_detected = !calculated_CRC.equals(received_CRC);


        if (error_detected) {
            System.out.println("Errors detected in the received data.");
        } else {
            System.out.println("No errors detected.");
        }


    }
}
