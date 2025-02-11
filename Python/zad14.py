# Потребно е да се напише Python скрипта за работа со крипто-валути. Скриптата добива на влез текстуална
# датотека во која, покрај другиот текст, се содржат вредности на крипто-валути. Скриптата треба да ја измине
# датотеката и за секоја одделна крипто-вредност да отпечати кое ќе биде најголемото зголемување од претходната
# вредност а кое ќе биде најголемото намалување од предходната вредност. На крај да се отпечатат, за секоја криптовалута одделно, 
# најголемото зголемување на валутата и помеѓу кои вредности е истото, најголемото намалување на
# валутата и помеѓу кои вредности е истото. Крипто-валути кои се земаат во предвид се: BTC, Doge, EOS, ETH и XPM. Во
# текстот најпрво се наоѓа кратенката за крипто-валутата а потоа вредноста во долари. Сите зборови се одделени помеѓу
# себе со празно место. Доколку некоја крипто-валута ја нема во датотеката или се појавува еднаш, не се печати ништо
# за таа крипто-валута.
# Пример:
# As of today, the cryptocurrency market is experiencing major price swings. Bitcoin (BTC) has seen a sharp rise to BTC $42300 a 15% increase
# over the past week BTC $48465 following positive news regarding institutional adoption. Ethereum (ETH) has also surged to ETH $2800
# benefiting from recent upgrades and increased usage in decentralized applications (dApps), and now is ETH $2300 .
# Other major coins have been fluctuating as well: Doge has jumped to Doge $350 a 10% increase, as the Binance ecosystem continues to
# expand globally Doge $385 Cardano (ADA), however, has fallen to EOS $0.38, losing 5% after a lack of significant development updates EOS
# $0.15 .
# List of all the changes:
# BTC $330000 fallen down to BTC $322500
# Doge $150.4 risen up to Doge $350
# На екран:
# BTC najgolemo pokacuvanje 28535 so vrednosti 48465 i 330000, najgolemo namaluvanje 7500 so vrednosti 330000 i 322500
# ETH najgolemo pokacuvanje 0 najgolemo namaluvanje 500 so vrednosti 2800 I 2300
# Doge najgolemo pokacuvanje 199.6 so vrednosti 150.4 I 350, najgolemo namaluvanje 234.6 so vrednosti 385 I 150.4
# EOS najgolemo pokacuvanje 0, najgolemo namaluvanje 0.23 so vrednosti 0.38 I 0.15

import sys

if len(sys.argv) < 2:
    print("Nevaliden broj na argumenti")
    sys.exit(1)

dat = open(sys.argv[1], "r")

crypto_vals = {"BTC": [], "ETH": [], "Doge": [], "EOS": [], "XPM": []}

for line in dat:
    zborovi = line.split()

    for i in range(len(zborovi) - 1):
        if zborovi[i] in crypto_vals and zborovi[i + 1].replace('$', '').replace('.', '').isdigit():
            price = float(zborovi[i + 1].replace('$', ''))
            crypto_vals[zborovi[i]].append(price)

dat.close()

for crypto, prices in crypto_vals.items():
    if len(prices) > 1:
        max_increase = 0
        max_decrease = 0
        max_increase_pair = ()
        max_decrease_pair = ()

        for i in range(1, len(prices)):
            razlika = prices[i] - prices[i - 1]
            if razlika > max_increase:
                max_increase = razlika
                max_increase_pair = (prices[i - 1], prices[i])
            if razlika < max_decrease:
                max_decrease = razlika
                max_decrease_pair = (prices[i - 1], prices[i])

        
    print(f"{crypto} najgolemo pokacuvanje {max_increase} so vrednosti {max_increase_pair[0]} i {max_increase_pair[1]}")
    print(f"{crypto} najgolemo namaluvanje {abs(max_decrease)} so vrednosti {max_decrease_pair[0]} i {max_decrease_pair[1]}")
