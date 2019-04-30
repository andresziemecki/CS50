<table class="table table-striped">

    <thead>
        <tr>
            <th>Transaction</th>
            <th>Symbol</th>
            <th>Shares</th>
            <th>Price</th>
            <th>Date/Time</th>
        </tr>
    </thead>

    <tbody>
        <?php foreach ($positions as $position): ?>

    <tr>
        <td><?= $position["type"] ?></td>
        <td><?= $position["symbol"] ?></td>
        <td><?= $position["shares"] ?></td>
        <td>$<?= $position["price"] ?></td>
        <td><?= $position["time"] ?></td>
    </tr>

        <?php endforeach ?>

    </tbody>

</table>