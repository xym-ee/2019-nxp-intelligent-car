adc_threshold = 650;
if adc_data > adc_threshold +50
    adc_data = 1;
    adc_threshold = adc_threshold -50;
end
if adc_data < adc_threshold -50
     adc_data = 0;
     adc_threshold = adc_threshold +50;
end
    