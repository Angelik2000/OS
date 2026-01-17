import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class CaesarCipher extends JFrame { //класс делаем окном

    private JTextArea inputArea;
    private JTextArea outputArea;
    private JTextField shiftField;

    public CaesarCipher() {
        setTitle("Шифратор Цезаря");
        setSize(600, 500);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout(10, 10));

        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.add(new JLabel(" Введите исходный текст:"), BorderLayout.NORTH);
        inputArea = new JTextArea(10, 50);
        inputArea.setLineWrap(true);
        inputArea.setWrapStyleWord(true);
        topPanel.add(new JScrollPane(inputArea), BorderLayout.CENTER);
        add(topPanel, BorderLayout.NORTH);

        JPanel centerPanel = new JPanel();
        centerPanel.add(new JLabel("Сдвиг:"));
        shiftField = new JTextField("3", 5);

        JButton encryptButton = new JButton("Зашифровать");
        JButton decryptButton = new JButton("Расшифровать");

        centerPanel.add(shiftField);
        centerPanel.add(encryptButton);
        centerPanel.add(decryptButton);
        add(centerPanel, BorderLayout.CENTER);

        JPanel bottomPanel = new JPanel(new BorderLayout());
        bottomPanel.add(new JLabel(" Результат:"), BorderLayout.NORTH);
        outputArea = new JTextArea(10, 50);
        outputArea.setLineWrap(true);
        outputArea.setWrapStyleWord(true);
        outputArea.setEditable(false);
        bottomPanel.add(new JScrollPane(outputArea), BorderLayout.CENTER);
        add(bottomPanel, BorderLayout.SOUTH);

        //Зашифровать
        encryptButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                processText(true);
            }
        });

        //Расшифровать
        decryptButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                processText(false);
            }
        });
    }

    private void processText(boolean isEncrypt) {
        String text = inputArea.getText();
        String shiftStr = shiftField.getText();

        try {
            int shift = Integer.parseInt(shiftStr);
            String result;

            if (isEncrypt) {
                result = encrypt(text, shift);
            } else {
                result = decrypt(text, shift);
            }
            outputArea.setText(result);

        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this,
                    "Ошибка! В поле 'Сдвиг' нужно ввести целое число.",
                    "Ошибка ввода",
                    JOptionPane.ERROR_MESSAGE);
        }
    }

    //метод шифрования
    public String encrypt(String text, int shift) {
        StringBuilder result = new StringBuilder();

        for (char character : text.toCharArray()) {
            if (character >= 'A' && character <= 'Z') {
                char shifted = (char) ('A' + (character - 'A' + shift) % 26);
                if (shifted < 'A') shifted += 26;
                result.append(shifted);
            } else if (character >= 'a' && character <= 'z') {
                char shifted = (char) ('a' + (character - 'a' + shift) % 26);
                if (shifted < 'a') shifted += 26;
                result.append(shifted);
            } else if (character >= 'А' && character <= 'Я') { // Русский
                char shifted = (char) ('А' + (character - 'А' + shift) % 32);
                if (shifted < 'А') shifted += 32;
                result.append(shifted);
            } else if (character >= 'а' && character <= 'я') { // Русский малые
                char shifted = (char) ('а' + (character - 'а' + shift) % 32);
                if (shifted < 'а') shifted += 32;
                result.append(shifted);
            } else {
                result.append(character);
            }
        }
        return result.toString();
    }

    //метод дешифрования
    public String decrypt(String text, int shift) {
        return encrypt(text, -shift); // Просто сдвигаем обратно
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new CaesarCipher().setVisible(true);
            }
        });
    }
}
