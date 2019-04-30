<table class="table table-striped" align="left">

    <thead>
        <tr>
            <th>Symbol</th>
            <th>Name</th>
            <th>Shares</th>
            <th>Price</th>
            <th>TOTAL</th>
        </tr>
    </thead>

    <tbody>
        <?php foreach ($positions as $position): ?>

    <tr>
        <td><?= $position["symbol"] ?></td>
        <td><?= $position["name"] ?></td>
        <td><?= $position["share"] ?></td>
        <td>$<?= $position["price"] ?></td>
        <td>$<?= $position["share"]*$position["price"] ?></td>
    </tr>

        <?php endforeach ?>

    <tr>
        <td>CASH</td>
        <td></td>
        <td></td>
        <td></td>
        <td>$<?= number_format($cash[0]["cash"], 2, '.', ' '); ?></td>
    </tr>

    </tbody>

</table>
<!-- 
    <div> 
        <iframe allowfullscreen frameborder="0" height="315" src="https://www.youtube.com/embed/oHg5SJYRHA0?autoplay=1&iv_load_policy=3&rel=0" width="420"></iframe> 
    </div>
-->
