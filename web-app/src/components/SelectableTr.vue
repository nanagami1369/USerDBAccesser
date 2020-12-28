<template>
  <tr @click="changeSelect" :class="{ selected: isSelect }">
    <slot />
  </tr>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator'

@Component
export default class UserInfoTable extends Vue {
  @Prop() public selectKey?: string
  private isSelect = false

  public changeSelect(): void {
    this.isSelect = !this.isSelect
    if (this.isSelect) {
      this.$emit('addKey', this.selectKey)
    } else {
      this.$emit('removeKey', this.selectKey)
    }
  }
}
</script>

<style scoped>
.selected td {
  background-color: orange;
}
</style>
