names = [{'date': '2024-01-01'}, {'date': '2024-01-02'}]
recent = sorted(names, key=lambda x: x['date'])
print(recent)
