<?php if (!empty($elements['elements']) && !empty($elements['elements'][0])): ?>
<div id="<?php print $elements['title'] ?>-wrapper" class="element">
  <?php foreach ($elements['elements'] as $element): ?>
    <?php print $element; ?>
  <?php endforeach; ?>
</div>
<?php endif;
