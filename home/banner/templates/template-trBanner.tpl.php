
<?php global $base_url; ?>
<?php if(isset($content['data']) && !empty($content['data'])): ?>
  <?php foreach ($content['data'] as $item): ?>
  <?php if( isset($item['video']) && !empty($item['video']) ): ?>
    <div class="main-banner-big slide-1">
      <iframe src="<?php print $item['video']; ?>?showinfo=0&amp;controls=0&amp;rel=0" allowfullscreen></iframe>
    <?php else: ?>
      <div class="main-banner-big slide-1" style="background: url('<?php print $item['img']; ?>') no-repeat top center;">
    <?php endif; ?>

    <a class="logo" href="<?php print $base_url; ?>">
      <div style="background: url('<?php print $item['logo']; ?>') no-repeat top center;">&nbsp;</div>
      <h1>Tía Rosa</h1></a>
    <div class="description-banner">
      <div class="container">
        <div class="row">
          <div class="col-12">

            <?php if( isset($item['title']) && !empty($item['title']) ): ?>
            <h2><?php print t($item['title']); ?></h2>
            <?php endif; ?>
            <?php if( isset($item['desc']) && !empty($item['desc']) ): ?>
            <h3><?php print t($item['desc']); ?></h3>
            <?php endif; ?>
          </div>
        </div>
      </div>
      <?php if( isset($item['class']) && !empty($item['class']) ): ?>
      <div class="clock"><span class="<?php print $item['class']; ?>"></span>
        <?php endif; ?>
        <?php if( isset($item['momento']) && !empty($item['momento']) ): ?>
        <p><?php print t($item['momento']); ?></p>
        <?php endif; ?>
      </div>
    </div>
  </div>
<?php endforeach; ?>
<?php endif; ?>
